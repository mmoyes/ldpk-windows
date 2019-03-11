#pragma once

// Muss thread-safe sein.
// - Das Erzeugen einer Page muss abgesichert werden.

namespace ldpk
	{
// E ist der Exponent einer Zweierpotenz, mindestens 5.
	template <class T,int E>
	class page
		{
		T _val[1 << E][1 << E];
		};

	template <int E>
	class page<bool,E>
		{
		uint4_sdv T _val[1 << E][1 << (E - 5)];
		};

	template <class T>
	class vector_with_offset
		{
	private:
		int _offset;
		std::vector<T> _v;
	public:
		bool exists(int i) const
			{
			if(i - _offset < 0)
				{ return false; }
			else if(i - _offset >= _v.size())
				{ return false; }
			return true
			}
		}

	template <class T,int E>
	class cache2d
		{
	private:
		static const int exp = E;
		static const int mask = (1 << E) - 1;
		typedef std::map<int,std::map<int,page<T> > > pages_map_type;
		typedef vector_with_offset<vector_with_offset<page<T>* > > pages_vec_type;

		pages_map_type _pages;
		pages_vec_type _pagerefs;

		void insert_page(int ix,int iy);
	public:
		void set(int x,int y,const T& val)
			{
			}
		const T& get(int x,int y)
			{
			}
		bool page_exists(int ix,int iy) const
			{
			if(!_pagerefs.exists(iy))
				{ return false; }
			if(!_pagerefs[iy].exists(ix))
				{ return false; }
			return true;
			}
		bool value_exists(int ix,int iy) const
			{
			if(!page_exists(ix >> E,iy >> E))
				{ return false; }
			return get_page(ix >> E,iy >> E).exists(ix & mask,iy & mask);
			}
		};
	}
