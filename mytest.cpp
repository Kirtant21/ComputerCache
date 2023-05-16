// CMSC 341 - Fall 2022 - Project 4
#include "cache.h"
#include <random>
#include <vector>
const int MINSEARCH = 0;
const int MAXSEARCH = 7;
// the following array defines sample search strings for testing
string searchStr[MAXSEARCH+1]={"c++","python","java","scheme","prolog","c#","c","js"};
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{

    public:
    void nonCollidingTest();
    void collidingTest();
};

unsigned int hashCode(const string str);

int main(){
    
    vector<Person> dataList;
    Random RndID(MINID,MAXID);
    Random RndStr(MINSEARCH,MAXSEARCH);
    Cache cache(MINPRIME, hashCode);
    Tester unitTest;
    bool result = true;
    for (int i=0;i<49;i++){
        // generating random data
        Person dataObj = Person(searchStr[RndStr.getRandNum()], RndID.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the Cache object
        cache.insert(dataObj);
    }
    // dumping the data points to the standard output
    cache.dump();
    // checking whether all data are inserted
    for (vector<Person>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == cache.getPerson((*it).getKey(), (*it).getID()));
    }
    if (result)
        cout << "All data points exist in the Cache object!\n";
    else
        cout << "Some data points are missing in the Cache object\n";
   
    cout << endl;
    cout << endl;
    cout << "************** Unit Test *****************" << endl;
    cout << "*******************************************************" << endl;

    unitTest.nonCollidingTest();
    unitTest.collidingTest();

    return 0;
}

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for ( unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

void Tester::nonCollidingTest(){

    cout << endl;
    cout << "TESTING - NON COLLIDING" << endl;
    cout << "*******************************************************" << endl;
    cout << endl;
    cout << endl;

    int temp = 0;
    string tempKey = "";
    string tempKeyHolder[20];
    //string insertionString[] = {"test1 insert", "test2 insert","test3 insert","test4 insert","test5 insert","test6 insert","test7 insert","test8 insert","test9 insert"};
    bool result = true;
    Random randID(MINID, MAXID);
    Random randKey(MINSEARCH,MAXSEARCH);
    Cache cacheTest(MINPRIME, hashCode);
    vector<Person> dataList;
    
    for (int i=0;i<30;i++){
        // generating random data
        temp =  randID.getRandNum();
        tempKey = searchStr[randKey.getRandNum()];
        Person dataObj = Person(tempKey,temp);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the Cache object
        cacheTest.insert(dataObj);
    }

     for (vector<Person>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == cacheTest.getPerson((*it).getKey(), (*it).getID()));
    }
    if (result)
        cout << "All data points exist in the Cache object!\n";
    else
        cout << "Some data points are missing in the Cache object\n";
   
    

    cout << endl;
    cout << endl;




    cout << "TESTING INSERT - NON COLLISION" << endl;
    
    unsigned int tempHash = hashCode(dataList[8].getKey());
    tempHash %= MINPRIME;

    cout << dataList[8].getKey() << " INSERTION PLACE (MUST BE): " << tempHash << endl;
    cout << endl;

    tempHash = hashCode(dataList[7].getKey());
    tempHash %= MINPRIME;

    cout << dataList[7].getKey() << " INSERTION PLACE (MUST BE): " << tempHash << endl;
    cout << endl;

    tempHash = hashCode(dataList[4].getKey());
    tempHash %= MINPRIME;

    cout << dataList[4].getKey() << " INSERTION PLACE (MUST BE): " << tempHash << endl;
    cout << endl;

    cout << "DUMP: INSERTED ITEMS FROM NON COLLISION" << endl;

    cacheTest.dump();

    cout << endl;

    cout << "TEST - SIZE OF HASH TABLE: " << cacheTest.m_currentSize << endl;
    cout << endl;

    cout << "TEST REMOVE (NON COLLISION)..... REMOVE INDEX:  " << dataList[0].getID() << ", " << dataList[0].getKey()<< endl;

    int dataList0ID = dataList[0].getID();
    string dataList0Key = dataList[0].getKey();

    if(cacheTest.remove(dataList[0]))
    {
        cout << "PERSON REMOVED SUCCESSFULLY! :)" << endl;

    }else{
        cout << "COULD NOT REMOVE THE PRESON! :(" << endl;
    }
    
    cout << endl;

    if(cacheTest.getPerson(dataList0Key, dataList0ID).getID() != dataList0ID)
    {
        cout << "TEST REMOVED PASSED SUCCESSFULLY! :)" << endl;
    }else{
        cout << "TEST REMOVED FAILED! :(" << endl;
    }

    cout << endl;

    int dataList1ID = dataList[1].getID();
    string dataList1Key = dataList[1].getKey();

    cout << "TEST(2) REMOVE (NON COLLISION)..... REMOVE INDEX:  " << dataList[1].getID() << ", " << dataList[1].getKey()<< endl;

    if(cacheTest.remove(dataList[1]))
    {
        cout << "PERSON REMOVED SUCCESSFULLY! :)" << endl;

    }else{
        cout << "COULD NOT REMOVE THE PERSON! :(" << endl;
    }

    cout << endl;

    if(cacheTest.getPerson(dataList1Key, dataList1ID).getID() != dataList1ID)
    {
        cout << "TEST REMOVED PASSED SUCCESSFULLY! :)" << endl;
    }else{
        cout << "TEST REMOVED FAILED! :(" << endl;
    }

    cout << endl;
    cout << "DUMPPING AFTER REMOVING 2 PERSONS.... :D" << endl;

    
    cacheTest.dump();


    cout<<endl;

    cout << "TESTING GET_PERSON().... :|" << endl;

    // TEST NORMAL CASE FOR GET Person FUNCTION

    if(cacheTest.getPerson(dataList[3].getKey(), dataList[3].getID()).getID() == dataList[3].getID())
    {
        cout << "TEST GET_PERSON NORMAL CASE - TEST PASSED! :)" << endl;    
    }else{
        cout << "TEST GET_PERSON NORMAL CASE - TEST FAILED! :(" << endl;
    }

    //Entering a Person that has been deleted already!
    if(cacheTest.getPerson(dataList1Key, dataList1ID).getID() == dataList1ID)
    {
        cout << "TEST GET_PERSON EDGE CASE - TEST FAILD! :(" << endl;    
    }else{
        cout << "TEST GET_PERSON  EDGE CASE- TEST PASSED! :)" << endl;
    }

    cout<<endl;

   

}

void Tester::collidingTest(){


    //*************************** Phase 1 ************************************************

    cout << endl;
    cout << "TESTING - COLLIDING" << endl;
    cout << "*******************************************************" << endl;
    cout << endl;
    cout << endl;

    vector<Person> dataList;
    Random randKey(MINSEARCH,MAXSEARCH);
    Random randID(MINID,MAXID);
    int tempIds[50];
    int tempIDHold =0;
    int index = 0;
    string tempKeyHold = "";
    string keyHoldTest[50];
    bool result = true;
    Cache cacheTest(MINPRIME, hashCode);

    // TEST INSERT COLLIDINGLY 

    for(int i = 0; i < 50; i++){
        //cout << "inside for loop" << endl;
        tempIDHold = randID.getRandNum();
        tempKeyHold = searchStr[randKey.getRandNum()];
        //cout << "tempKeyHold: " << tempKeyHold << endl;

        if(i % 3 == 0){
            keyHoldTest[index] = tempKeyHold;
            tempIds[index] = tempIDHold;
            index++;
        }

        if(i % 3 != 0){
           // cout << "inserting test1" << endl;
         Person testPerson = Person(tempKeyHold, tempIDHold);
         //cout << "temp: " << temp << endl;
         dataList.push_back(testPerson);
            cacheTest.insert(testPerson);
            //cout << "tempKeyHold: " << tempKeyHold << endl;
        }
        else{
           
            //cout << "tempKeyHold: " << tempKeyHold << endl;
            Person testPerson = Person(tempKeyHold, tempIDHold);
            cacheTest.insert(testPerson);
            dataList.push_back(testPerson);
        }
    }
  
  for (vector<Person>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == cacheTest.getPerson((*it).getKey(), (*it).getID()));
    }
    if (result)
        cout << "All data points exist in the Cache object!\n";
    else
        cout << "Some data points are missing in the Cache object\n";
   

    cout << endl;

    cout<< "DUMPING AFTER 50 INSERTIONS WITH MINPRIME" <<endl;
    cacheTest.dump();
    cout<<endl;

    //TEST REMOVE - COLLIDING 

    cout << "TEST REMOVAL - COLLISIDING KEYS WITHOUT TRIGERRING REHASH" << endl;
    
    for(int i = 0; i<14;i++){
        cacheTest.remove(Person(keyHoldTest[i],tempIds[i]));
    }

    cout << "REMOVED 14 PERSONS - DUMPING VALUES" << endl;
    cacheTest.dump();
    
    cout << endl;

    // *********************** Phase 2 *********************************


    cout <<"TESTING INSERT - COLLIDING WITH REHASH" << endl;
    Random randID2(MINID,MAXID);
    Random randKey2(MINSEARCH,MAXSEARCH);
    int tempID2[100];
    string tempKey[100];
    Cache tableRehash(MINPRIME, hashCode);
    int tempID = 0;
    int index2 = 0;
    string keyHold = "";
    vector<Person> dataList2;

    // TEST INSERT EDGE CASE - 100 INSERTS

    for(int i = 0; i <99; i++){
        //cout << "inside for loop i: " << i << endl;
        tempID = randID2.getRandNum();
        keyHold = searchStr[randKey2.getRandNum()];
        tempID2[index2] = tempID;
        tempKey[index2] = keyHold;
        //cout << "going to insert a person. tempID2[index2]: " << tempID2[index2] << endl;
        index2++;
        //cout << "temp2: " << temp2 << endl;
        Person testHuman = Person(keyHold, tempID);
        dataList2.push_back(testHuman);
        tableRehash.insert(testHuman);
       
           
    }

    // CHECKING TO SEE IF ALL THE DATA HAS BEEN INSERTED OR NOT

    for (vector<Person>::iterator it = dataList2.begin(); it != dataList2.end(); it++){
        result = result && (*it == tableRehash.getPerson((*it).getKey(), (*it).getID()));
    }
    if (result)
        cout << "All data points exist in the Cache object!\n";
    else
        cout << "Some data points are missing in the Cache object\n";
   
    cout << "99 PERSONS WERE INSERTED!" << endl;
    cout << "DUMPING VALUES" << endl;
    tableRehash.dump();
    cout << endl;

    // TEST REMOVE - EDGE CASE - REMOVING 84 ITEMS

    cout <<"TEST - REMOVE COLLISING (REHASHED)" << endl;

    for(int i = 0;i < 100; i++){
        tableRehash.remove(Person(tempKey[i], tempID2[i]));
    }
    cout << endl;
    cout << "FINAL DUMP - DONE REHASH AND REMOVAL" << endl;
    tableRehash.dump();



}