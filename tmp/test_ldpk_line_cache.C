#include <ldpk/ldpk_line_cache.h>
#include <iostream>
#include <iterator>


using namespace std;

void test_line_cache_buffer()
	{
	ldpk::cache_line_buffer<int> clb;

	clb.extend(15);
	for(int i = 15;i < 16;++i)
		{ clb[i] = i; }
	for(int i = clb.a();i < clb.b();++i)
		{ cout << i << ":" << clb[i] << " "; }
	cout << endl;
	copy(clb.begin(),clb.end(),ostream_iterator<int>(cout," "));
	cout << endl;
// Extend
	clb.extend(17);
	for(int i = 17;i < 18;++i)
		{ clb[i] = i; }
	for(int i = clb.a();i < clb.b();++i)
		{ cout << i << ":" << clb[i] << " "; }
	cout << endl;
	copy(clb.begin(),clb.end(),ostream_iterator<int>(cout," "));
	cout << endl;
// Mit resize erweitern
	clb.resize(13,19);
	for(int i = 13;i < 19;++i)
		{ clb[i] = i; }
	for(int i = clb.a();i < clb.b();++i)
		{ cout << i << ":" << clb[i] << " "; }
	cout << endl;
	copy(clb.begin(),clb.end(),ostream_iterator<int>(cout," "));
	cout << endl;
// Vorne erweitern
	clb.resize(11,15);
	for(int i = 11;i < 15;++i)
		{ clb[i] = i; }
	for(int i = clb.a();i < clb.b();++i)
		{ cout << i << ":" << clb[i] << " "; }
	cout << endl;
	copy(clb.begin(),clb.end(),ostream_iterator<int>(cout," "));
	cout << endl;
// Hinten erweitern
	clb.resize(17,22);
	for(int i = 17;i < 22;++i)
		{ clb[i] = i; }
	for(int i = clb.a();i < clb.b();++i)
		{ cout << i << ":" << clb[i] << " "; }
	cout << endl;
	copy(clb.begin(),clb.end(),ostream_iterator<int>(cout," "));
	cout << endl;
// Vorne und hinten erweitern
	clb.resize(9,24);
	for(int i = clb.a();i < clb.b();++i)
		{ cout << i << ":" << clb[i] << " "; }
	cout << endl;
	copy(clb.begin(),clb.end(),ostream_iterator<int>(cout," "));
	cout << endl;
	}

void test_line_cache()
	{
	ldpk::line_cache<int> cache;

	cache.resize_line(0,-15,+16);
	cout << cache << endl;

	cache.resize_line(1,-10,+20);
	cout << cache << endl;

	cache.resize_line(5,-5,+25);
	cout << cache << endl;

	cache.resize_line(-5,-25,+5);
	cout << cache << endl;

	cache.resize_line(-4,-24,+6);
	cout << cache << endl;

	cache.resize_line(6,-4,+26);
	cout << cache << endl;

	cout << "size: " << cache.size() << endl;
	}

int main()
	{
	test_line_cache();
	}
