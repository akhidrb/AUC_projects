#ifndef RanGen
#define RanGen

#define num 10
#define n 5

class RandGen {
public:
	RandGen();
	~RandGen();
	void sec();

private:
	int secret;
	int A[num];
	bool cont = true;
	int count = 0;
};
#endif