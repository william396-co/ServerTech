// sync_example.cpp: 定义应用程序的入口点。
//

#include "sync_example.h"
#include "safeQueue.h"

using namespace std;

#include <functional>
#include <future>
#include <thread>


using Func = std::function<void()>;
using FuncList = SafeQueue<Func>;


class Server {
public:
	template<typename F, typename...Args>
	auto add_func(F&& f, Args&&...args) -> std::future<decltype(f(args...))> {

		std::function<decltype(f(args)...))() > func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

		// wrap packaged_task into void function
		Func wrap_func = [task_ptr]() {
			(*task_ptr)();
		};

		m_funcList.enqueue(wrap_func);
		return task_ptr->get_future();
	}
	void run() {

		std::this_thread::sleep_for(std::chrono::milliseconds{200});

		Func func;
		while (!m_funcList.empty()) {
			auto dequeued = m_funcList.dequeue(func);
			if (dequeued)
				func();
		}
	}
private:
	FuncList m_funcList;
};

int main()
{
	Server s;

	std::thread t1([&s] {
		s.add_func([](int a, int b) {return a + b; }, 10, 20);
		});

	std::thread t2([&s] {
		s.add_func([](int a, int b) {return a * b; }, 10, 20);
		});
	s.run();


	t1.join();
	t2.join();

	return 0;
}
