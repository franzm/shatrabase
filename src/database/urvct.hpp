#ifndef __URVCT_H__
#define __URVCT_H__

template <typename T, int SIZE>
class urvct
{
  private:
    T items[SIZE];          // items
    int p;                  // pointer holds current number of items

  public:
    urvct();                // constructor
    void append(T const&);  // append item
    T& back();              // return last item
    const T& back() const;
    T& first();             // return first item
    const T& first() const; 
    bool isEmpty() const
    {
        return p == 0;      // return true if empty
    }
    bool isFull() const
    {
        return p == SIZE;   // return true if full
    }
    void clear()
    {
        p = 0;              // reset to empty
    }
    int count() const
    {
        return p;
    }
    T& operator[] (int i) { return items[i]; }  // array subscript
    const T& operator[] (int i) const { return items[i]; }
};

// constructor
template <typename T, int SIZE>
inline urvct<T,SIZE>::urvct ()
 : p(0)                      // initialize pointer
{}

template <typename T, int SIZE>
inline void urvct<T,SIZE>::append (T const& item) { items[p++] = item; }

template <typename T, int SIZE>
inline T& urvct<T,SIZE>::back () { return items[p - 1]; } // return last
template <typename T, int SIZE>
inline const T& urvct<T,SIZE>::back () const { return items[p - 1]; }

template <typename T, int SIZE>
inline T& urvct<T,SIZE>::first () { return items[0]; }    // return first
template <typename T, int SIZE>
inline const T& urvct<T,SIZE>::first () const { return items[0]; }

#endif
