#ifndef O_RANDOM_HPP
#define O_RANDOM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#include <stddef.h>
#include <assert.h>

//=====================================================================
// 32BIT INTEGER DEFINITION 
//=====================================================================
#ifndef __INTEGER_32_BITS__
#define __INTEGER_32_BITS__
#if defined(_WIN64) || defined(WIN64) || defined(__amd64__) || \
	defined(__x86_64) || defined(__x86_64__) || defined(_M_IA64) || \
	defined(_M_AMD64)
typedef unsigned int ISTDUINT32;
typedef int ISTDINT32;
#elif defined(_WIN32) || defined(WIN32) || defined(__i386__) || \
	defined(__i386) || defined(_M_X86)
typedef unsigned long ISTDUINT32;
typedef long ISTDINT32;
#elif defined(__MACOS__)
typedef UInt32 ISTDUINT32;
typedef SInt32 ISTDINT32;
#elif defined(__APPLE__) && defined(__MACH__)
#include <sys/types.h>
typedef u_int32_t ISTDUINT32;
typedef int32_t ISTDINT32;
#elif defined(__BEOS__)
#include <sys/inttypes.h>
typedef u_int32_t ISTDUINT32;
typedef int32_t ISTDINT32;
#elif (defined(_MSC_VER) || defined(__BORLANDC__)) && (!defined(__MSDOS__))
typedef unsigned __int32 ISTDUINT32;
typedef __int32 ISTDINT32;
#elif defined(__GNUC__)
#include <stdint.h>
typedef uint32_t ISTDUINT32;
typedef int32_t ISTDINT32;
#else 
typedef unsigned long ISTDUINT32;
typedef long ISTDINT32;
#endif
#endif


//=====================================================================
// Integer Definition
//=====================================================================
#ifndef __IINT8_DEFINED
#define __IINT8_DEFINED
typedef char IINT8;
#endif

#ifndef __IUINT8_DEFINED
#define __IUINT8_DEFINED
typedef unsigned char IUINT8;
#endif

#ifndef __IUINT16_DEFINED
#define __IUINT16_DEFINED
typedef unsigned short IUINT16;
#endif

#ifndef __IINT16_DEFINED
#define __IINT16_DEFINED
typedef short IINT16;
#endif

#ifndef __IINT32_DEFINED
#define __IINT32_DEFINED
typedef ISTDINT32 IINT32;
#endif

#ifndef __IUINT32_DEFINED
#define __IUINT32_DEFINED
typedef ISTDUINT32 IUINT32;
#endif

#ifndef __IINT64_DEFINED
#define __IINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 IINT64;
#else
typedef long long IINT64;
#endif
#endif

#ifndef __IUINT64_DEFINED
#define __IUINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 IUINT64;
#else
typedef unsigned long long IUINT64;
#endif
#endif

#ifndef INLINE
#if defined(__GNUC__)

#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define INLINE         __inline__ __attribute__((always_inline))
#else
#define INLINE         __inline__
#endif

#elif (defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__))
#define INLINE __inline
#else
#define INLINE 
#endif
#endif

#if (!defined(__cplusplus)) && (!defined(inline))
#define inline INLINE
#endif

typedef unsigned char byte;

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#define  platform_windows
#endif

#ifdef platform_windows
#include <windows.h>
#elif !defined(__unix)
#define __unix
#endif

#ifdef __unix
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#endif

#include <limits>
#include <vector>
#include <string>

/* get system time */
static inline void itimeofday(long *sec, long *usec)
{
#if defined(__unix)
	struct timeval time;
	gettimeofday(&time, NULL);
	if (sec) *sec = time.tv_sec;
	if (usec) *usec = time.tv_usec;
#else
	static long mode = 0, addsec = 0;
	BOOL retval;
	static IINT64 freq = 1;
	IINT64 qpc;
	if (mode == 0) {
		retval = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		freq = (freq == 0) ? 1 : freq;
		retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
		addsec = (long)time(NULL);
		addsec = addsec - (long)((qpc / freq) & 0x7fffffff);
		mode = 1;
	}
	retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
	retval = retval * 2;
	if (sec) *sec = (long)(qpc / freq) + addsec;
	if (usec) *usec = (long)((qpc % freq) * 1000000 / freq);
#endif
}

/* get clock in millisecond 64 */
static inline IINT64 iclock64(void)
{
	long s, u;
	IINT64 value;
	itimeofday(&s, &u);
	value = ((IINT64)s) * 1000 + (u / 1000);
	return value;
}

static inline IUINT32 iclock()
{
	return (IUINT32)(iclock64() & 0xfffffffful);
}

/* sleep in millisecond */
static inline void isleep(unsigned long millisecond)
{
#ifdef __unix 	/* usleep( time * 1000 ); */
	struct timespec ts;
	ts.tv_sec = (time_t)(millisecond / 1000);
	ts.tv_nsec = (long)((millisecond % 1000) * 1000000);
	/*nanosleep(&ts, NULL);*/
	usleep((millisecond << 10) - (millisecond << 4) - (millisecond << 3));
#elif defined(_WIN32)
	Sleep(millisecond);
#endif
}

static inline void osrandom(uint32_t seed) {
#ifdef platform_windows
	srand((unsigned int)seed);
#else
	srandom((unsigned int)seed);
#endif
}

static inline uint32_t orandom() {
#ifdef platform_windows
	return rand();
#else
	return random();
#endif
}

static inline std::vector<int> orandomv(int size, int num) {

	std::vector<int> rindex;
	if (size < num)
		return rindex;

	while (rindex.size() < num){

loop:
		osrandom(iclock());
		int index = orandom() % size;

		for (size_t i = 0; i < rindex.size(); i++) {
			if (rindex[i] == index)
				goto loop;
		}

		rindex.push_back(index);
	}
	
	return rindex;
}

template <typename T>
class COrandom{

	struct Tdata {

		T data;
		int count;
	};
public:
	COrandom() {}
	~COrandom() {}

	void push_data(T data) {

		for (auto it = m_data.begin(); it != m_data.end(); it++) {
			if (it->data == data)
				return;
		}

		struct Tdata s;
		s.data = data;
		s.count = 0;
		m_data.push_back(s);
	}

	void get_rindex(int num, std::vector<int>& rind) {

		int min = INT_MAX;
		// search min count
		for (auto it = m_data.begin(); it != m_data.end(); it++) {
			if (it->count < min)
				min = it->count;
		}

		// get min index
		std::vector<int> minv;
		for (size_t i = 0; i < m_data.size(); i++) {
			if (m_data[i].count == min)
				minv.push_back(i);
		}
		
		// delete selected index
		for (size_t i = 0; i < rind.size(); i++) {
			for (auto it = minv.begin(); it != minv.end(); it++) {
				if (*it == rind[i]) {
					minv.erase(it);
					break;
				}
			}
		}
	
		// get rand index
		if (minv.size() == num) {
			for (size_t i = 0; i < minv.size(); i++) {
				m_data[minv[i]].count++;
				rind.push_back(minv[i]);
			}
		}
		else if (minv.size() > num){
			std::vector<int> rindex = orandomv(minv.size(), num);
			for (size_t i = 0; i < rindex.size(); i++) {
				m_data[minv[rindex[i]]].count++;
				rind.push_back(minv[rindex[i]]);
			}
		}
		else{
			for (size_t i = 0; i < minv.size(); i++) {
				m_data[minv[i]].count++;
				rind.push_back(minv[i]);
			}

			get_rindex(num - minv.size(), rind);
		}
	}

	std::vector<T> get_rdata(int num) {
		std::vector<T> rdata;
		if (m_data.size() < num)
			return rdata;

		std::vector<int> rindex;
		get_rindex(num, rindex);
		printt(rindex);

		for (auto it = rindex.begin(); it != rindex.end(); it++) {
			rdata.push_back(m_data[*it].data);
		}

		return rdata;
	}

	void printt(std::vector<int>& vec) {
		std::cout << "=======================" << std::endl;
		for (auto it = vec.begin(); it != vec.end(); it++) {
			std::cout << "data: " << m_data[*it].data << "   " << "count: " << m_data[*it].count << std::endl;
		}
		std::cout << std::endl;
	}

private:
	std::vector<Tdata> m_data;
};

#endif // O_RANDOM_HPP