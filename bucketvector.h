#include <vector>
template<class T>
#define MaxIdxInVecLimit 0x0000FFFF
class bucketvector {
public:
	bucketvector() {
		MaxIdxInVec = 1<<8;
		while (MaxIdxInVec*sizeof(T)<MaxIdxInVecLimit) {
			MaxIdxInVec = MaxIdxInVec<<1;
		}
		_size = 0;
	}
	void push_back(const T& tt) {
		int idx1 = _size/MaxIdxInVec;
		int idx2 = _size%MaxIdxInVec;
		_size++;
		if (idx1==0) {
			v0.push_back(tt);
			return;
		}
		if (idx2==0) {
			T* pTT = (T*)malloc(MaxIdxInVec*sizeof(T));
			if (pTT==NULL) {
				throw std::bad_alloc();
			}
			store.push_back( pTT );
			pTT[0] = tt;
		} else {
			store[idx1-1][idx2] = tt;
		}
	}
	void clear() {
		size_t len = store.size();
		for (signed int i=len-1; i>=0; i--) {
			size_t elemCountInThisBucket = MaxIdxInVec;
			if (((i+1)*MaxIdxInVec)>=_size) elemCountInThisBucket = _size%MaxIdxInVec;
			for (signed int j=elemCountInThisBucket-1; j>=0; j--) {
				// Formation of below line was helped by deleteRange method from the page
				// http://www.altdevblogaday.com/2013/12/20/roll-your-own-vector/
				store[i][j].~T();
			}
			_size -= elemCountInThisBucket;
			free (store[i]);
			store.pop_back();//yes, either this or later clear can be used - using both neway
		}
		store.clear();
		v0.clear();
	}
	~bucketvector() {
		clear();
	}
	T& operator[] (size_t ii) { return _at(ii); }
	const T& operator[] (size_t ii) const { return _at(ii); }
	size_t size() const {
		return (size_t)_size;
	}
private:
	inline T& _at(size_t ii) {
		int idx1 = ii/MaxIdxInVec;
		int idx2 = ii%MaxIdxInVec;
		if (idx1==0) return v0[idx2];
		return store[idx1-1][idx2];
	}
	int MaxIdxInVec;
	std::vector<T> v0;
	int _size;
	std::vector<T*> store;
};
