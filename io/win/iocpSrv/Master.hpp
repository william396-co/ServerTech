#pragma once

class Master
{
public:
	Master();
	~Master();

	bool Run(int argc, char** argv);

	void _OnSignal(int s);
private:
	bool m_stopEvent;
};