#ifndef MARTY_READERSWRITERS_H
#define MARTY_READERSWRITERS_H


namespace marty
{

namespace rwu // RW Utils
{

// TCS must have methods:
// void lock()
// void unlock()
template <typename TCS>
class CGenericAutoLocker
{
    TCS      &Locker;
    public:
    CGenericAutoLocker(TCS &l) : Locker(l) { Locker.lock(); }
    ~CGenericAutoLocker() { Locker.unlock(); }
};


// TFlag must have methods:
// void set()         - set signaled state
// void clear()       - clear signaled state
// void passTrough()  - wait while state is signaled and pass trough if non-signaled

// TCounter must have methods:
// void increment()   - increment counter value
// void decrement()   - decrement counter value
// void passTrough()  - wait until counter is zero, pass trough if zero

template < typename TFlag
         , typename TCounter
         >
class CSingleWriterMultipleReadersLocker
{
    TFlag      readerLock;
    TCounter   readersCounter;

    public:

    CSingleWriterMultipleReadersLocker() 
       : readerLock(), readersCounter() {}

    CSingleWriterMultipleReadersLocker( const TFlag &f, const TCounter &c) 
       : readerLock(f), readersCounter(c) {}

    void enterReader()
       {
        readerLock.passTrough();    // check reader flag 
        readersCounter.increment();
       }

    void leaveReader()
       {
        readersCounter.decrement();
       }

    // writer must be single,
    // if multiple concurent writers try to have got access, behavior is undefined
    void enterWriter()
       {
        readerLock.set();  // all readers stops on readerLock.passTrough()
        readersCounter.passTrough(); // wait zero readers
       }

    void leaveWriter()
       {
        readerLock.clear();
       }
};

template <typename RWLocker>
class CReaderAutoLocker
{
    RWLocker  &Locker;
    public:
    CReaderAutoLocker(RWLocker &rwl) 
       : Locker(rwl) 
       { 
        Locker.enterReader();
       }
    ~CReaderAutoLocker() 
       { 
        Locker.leaveReader();
       }
};

template <typename RWLocker>
class CWriterAutoLocker
{
    RWLocker  &Locker;
    public:
    CWriterAutoLocker(RWLocker &rwl) 
       : Locker(rwl) 
       { 
        Locker.enterWriter();
       }
    ~CWriterAutoLocker() 
       { 
        Locker.leaveWriter();
       }
};




// TStorage must have methods:
// void swap(TStorage &)

template < typename TCS /* Critical section */
         , typename TFlag
         , typename TCounter
         , typename TStorage /* some container */
         , typename TStorageConstructorObject
         , typename TStorageDestructObject
         >
class CMultipleReadersWritersDataHolder
{
    mutable CSingleWriterMultipleReadersLocker<TFlag, TCounter>  readersWritersLocker;
    mutable TCS                                                  writerLockCs; // multiple writers must wait 
    TStorage                                                     data;
    TStorage                                                     shadowData;

    public:

    CMultipleReadersWritersDataHolder() : readersWritersLocker(), writerLockCs(), data(), shadowData() 
       {
        TStorageConstructorObject constructorObj;
        TStorageConstructorObject(data);
        TStorageConstructorObject(shadowData);
       }

    CMultipleReadersWritersDataHolder( const TCS &cs, const TFlag &f, const TCounter &c) 
       : readersWritersLocker(f,c), writerLockCs(cs), data(), shadowData() 
       {
        TStorageConstructorObject constructorObj;
        TStorageConstructorObject(data);
        TStorageConstructorObject(shadowData);
       }

    ~CMultipleReadersWritersDataHolder() 
       {
        TStorageDestructObject destructorObj;
        CGenericAutoLocker<TCS> locker(writerLockCs); // multiple writers must wait       
        destructorObj(shadowData);
        {
         CWriterAutoLocker< CSingleWriterMultipleReadersLocker<TFlag, TCounter> > locker(readersWritersLocker);
         shadowData.swap(data);
        }
        destructorObj(shadowData);    
       }

    template< typename ReadOperator >
    void readData( ReadOperator &op) const 
       { 
        CReaderAutoLocker< CSingleWriterMultipleReadersLocker<TFlag, TCounter> > locker(readersWritersLocker);
        op(data);
       }

    template< typename ReadOperator >
    void readData( const ReadOperator &op) const 
       { 
        CReaderAutoLocker< CSingleWriterMultipleReadersLocker<TFlag, TCounter> > locker(readersWritersLocker);
        op(data);
       }

    template< typename WriteOperator >
    void writeData( const WriteOperator &op)
       {
        CGenericAutoLocker<TCS> locker(writerLockCs); // multiple writers must wait       
        op(shadowData); // update shadow copy (long process, readers can read)
        {
         CWriterAutoLocker< CSingleWriterMultipleReadersLocker<TFlag, TCounter> > locker(readersWritersLocker); // wait no readers working with data
         shadowData.swap(data); // fast switching
        }
        op(shadowData); // long process, readers can read
       }
};


// use as TStorageConstructorObject/TStorageDestructObject
struct CDoNothing
{
    CDoNothing() {}

    template <typename TContainer>
    void operator()( TContainer & c) {}
    //void operator()( TContainer & c) {}
};


}; // namespace rwu
}; // namespace marty


#endif /* MARTY_READERSWRITERS_H */

