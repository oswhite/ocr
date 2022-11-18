// Big Integer Class header file
#include <string>
#include <vector>


class BigInt {
public:
    BigInt(std::string s, int abase=10); // convert string to BigInt
    std::string to_string() const; // get string representation
    void add(BigInt b); // add another BigInt to this one

    // returns the sum of this BigInt and rhs
    BigInt operator+(const BigInt& rhs) const; 
    // returns the difference of this BigInt minus rhs
    BigInt operator-(const BigInt& rhs) const; 
    // returns the true if this BigInt is less than rhs
    bool operator<(const BigInt& rhs) const; 
private:
    // whatever you need
    void removeLeadingZeroes(); //remove leading 0s
    std::vector<int> vec;
    int base;

};

