#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdlib.h>


const unsigned BASE = 10, MAXP = 9, MAXN = 1000'000'000, MAXSUM = (BASE - 1) * MAXP;

int nwsarr[MAXSUM+1][BASE][MAXP]; // sum k p
int nwsNarr[MAXSUM+1];
short digits[MAXP];
short len = 0;
short dsum = 0;
int n;

int nws(short sum = 0, short k = 0, short p = 1)
// Возвращает количество чисел с суммой sum
// от k*10^p до (k+1)*10^p - 1
// 0 <= sum <= MAXSUM
// 0 <= k < BASE
// 0 <= p <= MAXP, но если p == MAXP, то k только 0.
{
	short mirsum = 0;
	if (p == MAXP) // k == 0;
	{
		int res = 0;
		for (short i = 0; i < BASE; ++i) res += nws(sum, i, MAXP-1);
		return res;
	} else if (sum < 0)
	{
		return 0;
	} else if (sum < k)
	{
		return nwsarr[sum][k][p] = 0;
	} else if (nwsarr[sum][k][p] >= 0)
	{
		return nwsarr[sum][k][p];
	} else if ((mirsum = 2*k + (BASE-1)*p - sum) < sum)
	{
		return nwsarr[sum][k][p] = nws(mirsum, k, p);
	} else if (k == 0)
	{
		int res = 0;
		for (int i = 0; i < BASE; ++i) res += nws(sum-i, 0, p-1);
		return nwsarr[sum][k][p] = res;
	} else {
		return nwsarr[sum][k][p] = nws(sum-k, 0, p);
	}
}

int nwsN(short sum = 0, unsigned nn = 0)
{
	if (nn)
	{
		short cdigits[MAXP];
		short clen = 0;
		short cdsum = 0;
		if (nn == MAXN) return nws(sum, 0, MAXP) + (sum == 1);
		while (nn > 0)
		{
			auto dm = ldiv(nn, BASE);
			cdsum += cdigits[clen] = dm.rem;
			nn = dm.quot;
			++clen;
		}
		int res = 0;
		short sumbeh = cdsum;

		if (sumbeh == sum) ++res;
		for (short pos = 0; pos < clen; ++pos)
		{
			sumbeh -= cdigits[pos];
			for (short k = 0; k < cdigits[pos]; ++k)
				res += nws(sum - sumbeh, k, pos);
		}
		return res;
	} else {
		if  // n == 1000000000
			(digits[0] == -1) return nws(sum, 0, MAXP) + (sum == 1);

		int res = 0;
		short sumbeh = dsum;

		if (sumbeh == sum) ++res;
		for (short pos = 0; pos < len; ++pos)
		{
			sumbeh -= digits[pos];
			for (short k = 0; k < digits[pos]; ++k)
				res += nws(sum - sumbeh, k, pos);
		}
		return res;
	}
}

short digsum(unsigned int n)
{
	short res = 0;
	while (n > 0)
	{
		auto dm = ldiv(n, BASE);
		res += dm.rem;
		n = dm.quot;
	}
	return res;
}

int main(int argc, char** argv){
	n = atoi(argv[1]);

	if (n == MAXN) digits[0] = -1;
	else while (n > 0)
	{
		auto dm = div(n, BASE);
		dsum += digits[len] = dm.rem;
		n = dm.quot;
		++len;
	}

	// PREPARE
	for (short sum=0; sum<=81; ++sum)
		for (short k=0; k<=9; ++k)
			for (short p=0; p<=8; ++p)
				nwsarr[sum][k][p] = -1;

	for (short i=0; i<=9; ++i) nwsarr[i][i][0] = 1;
	// for (short k=0; k<=9; ++k)
	// 	for (short sum=k; sum<=k+9; ++sum)
	// 		nwsarr[sum][k][1] = 1;
	// for (short k=0; k<=9; ++k)
	// 	for (short sum=k; sum<=k+18; ++sum)
	// 		nwsarr[sum][k][2] = 10 - abs(10 - (sum-k+1));

	// MAIN
	short oversum = 1;
	nwsNarr[0] = 0;
	while (oversum <= 81)
	{
	 	nwsNarr[oversum] = nwsNarr[oversum-1] + nwsN(oversum);
	 	if (nwsNarr[oversum] == nwsNarr[oversum-1]) break;
	 	++oversum;
	}

	int qmin, qmid, qmax, fq;
	short fixed = 1; // 1 - неподвижная точка
	for (short sum = 2; sum < oversum; ++sum)
	{

		qmax = nwsNarr[sum];
		fq = qmax - nwsN(sum, qmax) - nwsNarr[sum-1];
		if (fq == 0 && digsum(qmax) == sum)
		{
			++fixed;
			continue;
		} else if (fq > 0)
		{
			qmin = nwsNarr[sum-1] + 1;
			fq = qmin - nwsN(sum, qmin) - nwsNarr[sum-1];
			// Надеемся, что fmin <= 0 всегда.
			if (fq == 0 && (digsum(qmin) == sum || digsum(qmin+1) == sum))
			{
				++fixed;
				continue;
			}
			while (1)
			{
				if (qmin + 1 == qmax) break;
				qmid = (qmin + qmax) / 2;
				fq = qmid - nwsN(sum, qmid) - nwsNarr[sum-1];
				if (fq < 0) qmin = qmid;
				else if (fq == 0)
				{
					if (digsum(qmid) == sum || (digsum(qmid+1) == sum && qmid < qmax))
						++fixed;
					break;
				}
				else qmax = qmid;
			}
		}
	}
	std::cout << fixed << std::endl;

	return 0;
}
