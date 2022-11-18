// Big Integer Class Implementation
#include <iostream>
#include <string>
#include <vector>
#include "bigint.h"

using namespace std;

// Write your implementation below
BigInt::BigInt(string s, int abase)
{
  for(int i = 0; i < s.length(); i++){
    char c = s[s.length()-(1+i)];
    int x;
    if(int(c) < 58){
      x = static_cast<int>(c) - '0';
    }
    else{
      x = 10 + static_cast<int>(c) - 'A';
    }
    vec.push_back(x);
  }
  base = abase;
  this->removeLeadingZeroes();
}
void BigInt::removeLeadingZeroes()
{
  //if vector is just 0
  if(this->vec.size() == 1){
    return;
  }
  //otherwise
  while(this->vec.back() == 0){
    this->vec.pop_back();
    if(this->vec.size() == 1){
      break;
    }
  }
}

string BigInt::to_string() const
{
  string s = "";
  for(int i = vec.size() -1; i >=0; i--){
    char c;
    if(vec[i] < 10){
      c = (char)(vec[i] + '0');
    }
    else{
      c = (char)(vec[i] - 10 + 'A');
    }
    s+= c;
  }
  return s;
}

void BigInt::add(BigInt b)
{
  int diff;
  if(b.vec.size() > vec.size()){
    diff = b.vec.size() - vec.size();
    for(int i = 0; i < diff; i++){
      vec.push_back(0);
    }
  }
  else{
    diff = vec.size() - b.vec.size();
    for(int i = 0; i < diff; i++){
      b.vec.push_back(0);
    }
  }
  //
  int carryv = 0;
  for(int i = 0; i < vec.size(); i++){
    if(vec[i] + b.vec[i] + carryv < b.base){
      vec[i] = vec[i] + b.vec[i] + carryv;
      carryv = 0;
    }
    else{
      int temp = vec[i] + b.vec[i] + carryv;
      cout << "temp: " << temp << endl;
      vec[i] = temp % b.base;
      carryv = temp/b.base;
    }
  }
  if(carryv != 0){
    if(carryv == b.base){
      vec.push_back(carryv%b.base);
      vec.push_back(carryv/b.base);
    }
    else{
      vec.push_back(carryv);
    }
  }
}

// returns the sum of this BigInt and rhs
BigInt BigInt::operator+(const BigInt& rhs) const
{
  BigInt temp = *this;
  BigInt temprhs = rhs;
  int diff;
  if(temprhs.vec.size() > temp.vec.size()){
    diff = temprhs.vec.size() - temp.vec.size();
    for(int i = 0; i < diff; i++){
      temp.vec.push_back(0);
    }
  }
  else{
    diff = temp.vec.size() - temprhs.vec.size();
    for(int i = 0; i < diff; i++){
      temprhs.vec.push_back(0);
    }
  }
  
  int carryv = 0;
  for(int i = 0; i < temp.vec.size(); i++){
    if(temp.vec[i] + temprhs.vec[i] + carryv < temprhs.base){
      temp.vec[i] = temp.vec[i] + temprhs.vec[i] + carryv;
      carryv = 0;
    }
    else{
      int t = temp.vec[i] + temprhs.vec[i] + carryv;
      temp.vec[i] = t % temprhs.base;
      carryv = t/temprhs.base;
    }
  }
  if(carryv != 0){
    if(carryv == temprhs.base){
      temp.vec.push_back(carryv%temprhs.base);
      temp.vec.push_back(carryv/temprhs.base);
    }
    else{
      temp.vec.push_back(carryv);
    }
  }
  temp.removeLeadingZeroes();
  return temp;
}
// returns the difference of this BigInt minus rhs
BigInt BigInt::operator-(const BigInt& rhs) const
{
  //create temporary bigints for modification
  BigInt temp = *this;
  BigInt temprhs = rhs;
  int diff; //need difference in vec size
  if(temprhs.vec.size() > temp.vec.size()){
    diff = temprhs.vec.size() - temp.vec.size();
    for(int i = 0; i < diff; i++){
      temp.vec.push_back(0);
    }
  }
  else{
    diff = temp.vec.size() - temprhs.vec.size();
    for(int i = 0; i < diff; i++){
      temprhs.vec.push_back(0);
    }
  }
  
  //now all have same number of digits
  //proceed to subtraction
  for(int i = 0; i < temp.vec.size(); i++){
    if(temp.vec[i] - temprhs.vec[i] >= 0){
      temp.vec[i] = temp.vec[i] - temprhs.vec[i];
    }
    else{
      int t = temp.vec[i] + temp.base;
      temp.vec[i+1] -=1;
      temp.vec[i] = t - temprhs.vec[i];
    }
  }
  temp.removeLeadingZeroes();
  return temp;
}
// returns the true if this BigInt is less than rhs
bool BigInt::operator<(const BigInt& rhs) const
{
  //create temporary bigints for modification
  BigInt temp = *this;
  BigInt temprhs = rhs;
  //make sure neither have leading zeroes
  temp.removeLeadingZeroes();
  temprhs.removeLeadingZeroes();
  if(temp.vec.size() > temprhs.vec.size()){
    return false;
  }
  if(temp.vec.size() < temprhs.vec.size()){
    return true;
  }
  //otherwise if vectors are same size, proceed below
  int len = temp.vec.size(); //this will be size of both
  for(int i = len - 1; i >= 0; i--){
    if(temp.vec[i] > temprhs.vec[i]){
      return false;
    }
    if(temp.vec[i] < temprhs.vec[i]){
      return true;
    }
    //if number is same, next iteration of loop
  }
  //if bigints are exact same, return false
  return false;
}

