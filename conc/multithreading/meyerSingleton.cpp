class MySingleton
{
public:
    static MySingleton & getInstance()
    {
        static MySingleton instance;
        return instance;
    }

private:
    MySingleton() = default;
    ~MySingleton() = default;

    MySingleton( MySingleton const & ) = delete;
    MySingleton & operator=( MySingleton const & ) = delete;
};

int main()
{

    MySingleton::getInstance();

    return 0;
}

