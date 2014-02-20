#ifndef __URSTACK_H__
#define __URSTACK_H__

template <typename T, int SIZE> /* adapted from Vandevoorde and Josuttis */
class urstack
{
  private:
    T items[SIZE];          // items
    int sp;                 // stack pointer holds current number of items

  public:
    urstack();              // constructor
    void push(T const&);    // push item
    void pop();             // pop
    T top() const;          // return top item
    T remove();             // return top item & pop
//    T first() const;        // return first (bottom) item

    bool empty() const
    {
        return sp == 0;     // return true if stack empty
    }
    bool full() const
    {   
        return sp == SIZE;  // return true if stack full
    }
    void clear()
    {
        sp = 0;             // reset to empty
    }
    int count()
    {
        return sp;
    }
};

// constructor
template <typename T, int SIZE>
inline urstack<T,SIZE>::urstack ()
 : sp(0)                     // initialize stack pointer
{}                           // nada mas

template <typename T, int SIZE>
inline void urstack<T,SIZE>::push (T const& item) { items[sp++] = item; } // append

template<typename T, int SIZE>
inline void urstack<T,SIZE>::pop () { sp -= 1; }   // 'pop stack' = dec sp

template <typename T, int SIZE>
inline T urstack<T,SIZE>::top () const { return items[sp - 1]; } // return top

template <typename T, int SIZE>             // return top & pop
inline T urstack<T,SIZE>::remove () { return items[--sp]; }

//template <typename T, int SIZE>
//T urstack<T,SIZE>::first () const { return items[0]; }    // cheeky

#endif
