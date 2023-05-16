// CMSC 341 - Fall 2022 - Project #include "cache.h"
#include "cache.h"
Cache::Cache(int size, hash_fn hash){
    
    if(size > MAXPRIME)
    {
        size = MAXPRIME;
    }
    else if(size < MINPRIME)
    {
        size = MINPRIME;
    }

    m_currentCap = size;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_oldCap = 0;
    m_oldSize = 0;
    m_oldNumDeleted = 0;
    m_oldTable = nullptr;
    m_currentTable = new Person[m_currentCap];
    m_hash = hash;
    m_hashing = false;


}

Cache::~Cache(){

    m_hash = nullptr;
    m_currentCap = 0;
    m_currentSize = 0;
    m_currNumDeleted = 0;
    m_oldCap = 0;
    m_oldSize = 0;
    m_oldNumDeleted = 0;
    m_hashing = false;

    if(m_currentTable != nullptr)
    {
        delete [] m_currentTable;
    }

    if(m_oldTable != nullptr)
    {
        delete [] m_oldTable;
    }

}

bool Cache::insert(Person person){
    
    bool inserted = false;
    int hNum = m_hash(person.getKey());
    hNum %= m_currentCap;

    if(m_currentTable[hNum] == EMPTY) // adds if empty
    {
        m_currentTable[hNum] = person;
        m_currentSize++;
        inserted = true;
    }   
    else{
        
        for(int i = 0; i < m_currentCap && inserted == false; i++) // goes through the whole curr table
        {
            int hNum = ((m_hash(person.getKey()) % m_currentCap) + (i * i)) % m_currentCap; // quadratic probing
            if(m_currentTable[hNum] == EMPTY) // looking for empty spot
            {
                m_currentTable[hNum] = person;
                m_currentSize++;
                inserted = true;

            }
        }
    }
    
    if(!m_hashing) // checks if hashing 
    {
        if(lambda() > .5)
        {
            createCurrHashHelper();
            m_hashing = true;
        }
    }
    else {
        rehashOld();
    }
    return inserted;

}

bool Cache::remove(Person person){

    bool isRemoved = false;

    if(m_oldTable != nullptr)  // protects against seg fault  
    {
        for(int i = 0; i < m_oldCap; i++)
        {
            if(m_oldTable[i].getKey().empty() != true) // makes sure it's not empty 
            {
                if(m_oldTable[i] == person) // person matched or not
                {
                    m_oldTable[i] = DELETED; 
                    m_oldNumDeleted++;
                    isRemoved = true;
                }
            }
        }
    }
    
    if(!m_hashing)
    {
        if(oldDeletedRatioHelper() > .8)
        {
            m_hashing = true;
            createOldHashHelper();   
        }
       
    }
    else if(m_hashing){
        rehashCurrent();
    }        
    


    
    if(m_currentTable != nullptr)   // protects against seg fault and doing the same thing as code above except, this snippet is for curr
    {
        for(int i = 0; i < m_currentCap; i++)
        {
            if(m_currentTable[i].getKey().empty() == false)
            {
                if(m_currentTable[i] == person)
                {
                    m_currentTable[i] = DELETED;
                    m_currNumDeleted++;
                    isRemoved = true;
                }
            }
        }
    }
    
    if(!m_hashing && (deletedRatio() > .8))
    {
        m_hashing = true;
        createCurrHashHelper();
    }
    else{
        if(m_hashing)
        {
            rehashCurrent();
        }        
    }

    return isRemoved;

}

Person Cache::getPerson(string key, int id) const{
    
    if(m_oldTable != nullptr) // protecting from seg fault
    {
        for(int i = 0; i < m_oldCap; i++) // goes through the old table
        {
            if(m_oldTable[i].getKey().empty() != true) // makes sure it is not empty
            {
                if(m_oldTable[i].getID() == id && (m_oldTable[i].getKey() == key)) // key and id are matched or not
                {
                    return m_oldTable[i]; // returns if found
                }
            }
        }
    }

    // Code below is the excet same thing as the code above, except below code is for curr

    if(m_currentTable != nullptr)
    {
        for(int i = 0; i < m_currentCap; i++)
        {
            if(m_currentTable[i].getKey().empty() == false)
            {
                if(m_currentTable[i].getID() == id && (m_currentTable[i].getKey() == key)) 
                {
                    return m_currentTable[i];
                }
            }
        }
    }

    cout << "Operation failed: Person not found!" << endl; // Note to self: Not a debugging statment.
    //Person emptyHumanBeing; 
    return EMPTY; // if the scop reaches this line, it means person not found!



}

float Cache::lambda() const {

    return float(m_currentSize/m_currentCap);
}

float Cache::deletedRatio() const {
    
    int deleteNum = 0;
    float returnVal = 0;

    for(int i = 0; i < m_currentCap; i++) // goes through the table and deletes 
    {
        if(m_currentTable[i].getKey().empty() != true)
        {
            if(m_currentTable[i].getKey() == DELETEDKEY)
            {
                deleteNum++;
            }
        }
    }

 if(deleteNum != 0 && m_oldSize != 0) // to give protection against Floting Point Exception
    {
        returnVal = deleteNum / m_oldSize;
    }

    return returnVal;

}

void Cache::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool Cache::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int Cache::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

ostream& operator<<(ostream& sout, const Person &person ) {
    if (!person.m_key.empty())
        sout << person.m_key << " (ID " << person.m_id << ")";
    else
        sout << "";
  return sout;
}

bool operator==(const Person& lhs, const Person& rhs){
    return ((lhs.m_key == rhs.m_key) && (lhs.m_id == rhs.m_id));
}

float Cache::oldDeletedRatioHelper() const{

    int deleteNum = 0;
    float returnVal = 0;

    for(int i = 0; i < m_oldCap; i++) // goes through the table and deletes
    {
        if(m_oldTable[i].getKey().empty() != true) 
        {
            if(m_oldTable[i].getKey() == DELETEDKEY)
            {
                deleteNum++;
            }
        }
    }

    if(deleteNum != 0 && m_oldSize != 0) // to give protection against Floting-Point Exception
    {
        returnVal = deleteNum / m_oldSize;
    }


    return returnVal;

}

void Cache::createCurrHashHelper(){

    if(m_oldTable != nullptr){ 
        delete [] m_oldTable;
        m_oldTable = nullptr;
    }
    m_oldCap = ((m_currentSize = m_currNumDeleted) * 4);
    
    if(!isPrime(m_oldCap)){
        m_oldCap = findNextPrime(m_oldCap);
    }
    
    m_oldTable = new Person[m_oldCap];
    m_hashing = true;
         

}
void Cache::createOldHashHelper(){

    if(m_currentTable != nullptr) // protects from throwing seg fault
    { 
        delete [] m_currentTable; // deletes the table
        m_currentTable = nullptr;
    }

    m_currentCap = ((m_oldSize = m_oldNumDeleted) * 4); // resets the table with the old table
    
    if(!isPrime(m_currentCap))
    {
        m_currentCap = findNextPrime(m_currentCap);
    }
        m_currentTable = new Person[m_oldCap];
        m_hashing = true;   
}

void Cache::insertCurrHelper(Person person){

    int quadCnt = 0, hashNum = m_hash(person.getKey()) % m_currentCap; // does the math

    if(m_currentTable[hashNum].getKey().empty() != true)
    {
        while(m_currentTable[hashNum] == DELETED && (m_currentTable[hashNum].getKey().empty() != true)) // goes through the table with the hashNum which had some math done to it
        {
            hashNum += pow(quadCnt, 2);
            hashNum %= m_currentCap;
        }

        m_currentTable[hashNum] = person;
        m_currentSize++;
    }

}
void Cache::insertOldHelper(Person person){

 // same thing as insertCurrHelper except its for old table

    int quadCnt = 0, hashNum = m_hash(person.getKey()) % m_oldCap;

    if(m_oldTable[hashNum] == EMPTY)
    {
        while(m_oldTable[hashNum] == DELETED && (m_oldTable[hashNum].getKey().empty() == false))
        {
            hashNum += pow(quadCnt, 2);
            hashNum %= m_oldCap;
        }

        m_oldTable[hashNum] = person;
        m_oldSize++;
    }
}
void Cache::rehashOld(){

    int dNum = m_oldSize * .25; // 25% 
    int index = 0, current = 0;

    while((current <= dNum) && index < m_oldCap) // goes through the table, doing the probing
    {
        if(m_oldTable[index].getKey().empty() != true)
        {
            if(m_oldTable[index].getKey() != DELETEDKEY)
            {
                Person human(m_oldTable[index].getKey(), m_oldTable[index].getID());
                insertOldHelper(human);
                m_oldTable[index] = DELETED;
                current++;

            }
        }

        index++;
    }

    if(index == m_oldCap)
    {
        m_hashing = false;
        delete [] m_oldTable;
        m_oldCap = 0;
        m_oldSize = 0;
        m_oldTable = nullptr;
        m_oldNumDeleted = 0;
    }

}
void Cache::rehashCurrent(){

    int dNum = m_currentSize * .25; // 25%
    int index = 0, current = 0;

    while((current <= dNum) && index < m_currentCap) 
    {
        if(m_currentTable[index].getKey().empty() != true)
        {
            if(m_currentTable[index].getKey() != DELETEDKEY)
            {
                Person human(m_currentTable[index].getKey(), m_currentTable[index].getID());
                insertCurrHelper(human);
                m_currentTable[index] = DELETED;
                current++;

            }
        }

        index++;
    }

    if(index == m_currentCap)
    {
        m_hashing = false;
        delete [] m_currentTable;
        m_currentCap = 0;
        m_currentSize = 0;
        m_currentTable = nullptr;
        m_currNumDeleted = 0;
    }

}
