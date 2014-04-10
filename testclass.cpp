#include <vector>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "bucketvector.h"
using namespace std;
#define LenMyVectorElem 128
class MyVectorElem {
	int data[LenMyVectorElem];
	public:
	MyVectorElem(int ii) {
		for (int i=0; i<LenMyVectorElem; i++) data[i] = ii;
	}
	MyVectorElem(const MyVectorElem& that) {
		for (int i=0; i<LenMyVectorElem; i++) data[i] = that.data[i];
	}
	int equals(const MyVectorElem& that) {
		for (int i=0; i<LenMyVectorElem; i++) {
			if (data[i]!=that.data[i])
				return 1;
		}
		return 0;
	}
};
struct durs {
	long push_dur;
	long access_dur;
	long size;
};
template<class T1, class T2>
class TestClass {
	public:
	vector<durs> durVec;
	void testDurations() {
		durVec.clear();
		unsigned long maxValInit = 0x00FF;
		for (size_t i=0; i<64; i++) {
			unsigned long maxVal = maxValInit<<i;
			try {
				testOneDuration(maxVal);
			} catch (std::bad_alloc&) {
				break;
			}
		}
	}
	private:
	void testOneDuration(unsigned long MAX) {
		T1 tv;
		clock_t tv_pushback1 = clock();
		for (size_t i=0; i<MAX; i++) {
			try {
				tv.push_back(T2(i*i));
			} catch (std::bad_alloc&) {
				throw;
			}
		}
		clock_t tv_pushback2 = clock();
		T2 tmpElem(0);
		clock_t tv_access1 = clock();
		for (size_t i=0; i<MAX; i++) {
			tmpElem = tv[i];
		}
		clock_t tv_access2 = clock();
		durs theDur;
		theDur.push_dur   = ((tv_pushback2-tv_pushback1)/1000);
		theDur.access_dur = ((tv_access2  -tv_access1  )/1000);
		theDur.size = MAX;
		durVec.push_back(theDur);
	}
};
void testPerf() {
	TestClass<    vector<MyVectorElem>, MyVectorElem> tcvv;
	TestClass<bucketvector<MyVectorElem>, MyVectorElem> tcsv;
	tcvv.testDurations();
	tcsv.testDurations();
	size_t biggerSize = tcvv.durVec.size()>tcsv.durVec.size()?tcvv.durVec.size():tcsv.durVec.size();
	printf("numElems, vv.push, vv.access, sv.push, sv.access\n");
	for (size_t i=0; i<biggerSize; i++) {
		int durations[4] = {-1L, -1L, -1L, -1L};
		int thisSize = 0;
		if (i<tcvv.durVec.size()) { durations[0] = tcvv.durVec[i].push_dur; durations[1] = tcvv.durVec[i].access_dur; thisSize = tcvv.durVec[i].size; }
		if (i<tcsv.durVec.size()) { durations[2] = tcsv.durVec[i].push_dur; durations[3] = tcsv.durVec[i].access_dur; thisSize = tcsv.durVec[i].size; }
		printf("%d, %d, %d, %d, %d\n", thisSize, durations[0], durations[1], durations[2], durations[3]);
	}
}
void testBasic() {
	size_t MAX = 0x7FFF;
	vector<MyVectorElem> vv;
	bucketvector<MyVectorElem> sv;
	for (size_t i=0; i<MAX; i++) {
		int ii = rand();
		vv.push_back(MyVectorElem(ii));
		sv.push_back(MyVectorElem(ii));
	}
	//vv[0] = MyVectorElem(8);
	//sv[0] = MyVectorElem(rand());
	bool failed = false;
	for (int i=MAX-1; i>=0; i--) {
		MyVectorElem vvMyVectorElem = vv[i];
		MyVectorElem svMyVectorElem = sv[i];
		if ( 0!=vvMyVectorElem.equals(svMyVectorElem) || 0!=svMyVectorElem.equals(vvMyVectorElem) ) {
			printf("failed at i=%d\n", i);
			failed = true;
		}
	}
	if (!failed) {
		printf("seems ok - tested with vv.size=%d and sv.size=%d\n", vv.size(), sv.size());
	}
}
int main(int argc, char** argv) {
	testBasic();
	testPerf();
}
