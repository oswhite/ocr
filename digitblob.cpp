#include "digitblob.h"
#include <iostream>
#include <vector>

using namespace std;

bool Location::operator<(const Location& other) const
{
    if(this->col < other.col){
        return true;
    }
    else if(this->col > other.col){
        return false;
    }
    //if columns are same, go by row
    else{
        if(this->row < other.row){
            return true;
        }
        else{
            return false;
        }
    }
}

DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initialize h_ and w_ and any other data members
    h_ = 0;
    w_ = 0;


}

DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members
    ul_ = upperleft;
    h_ = height;
    w_ = width;


}

DigitBlob::~DigitBlob()
{
    

}

void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    calc_vertical_symmetry();
    calc_centers_of_mass();
    calc_aspect_ratio();
    isTopHeavy();
    isLeftHeavy();
    topHoSym();
    middleRatio();


    if(euler_ == -1){
        digit_ = '8';
    }
    else if(euler_ == 0){
        //could be 0, 4, 6, or 9
        if(vertsym_ > 0.80){
            digit_ = '0';
        }
        //need to take four out here
        else if(vertsym_ >= 0.7 && vertsym_ < 0.8 && !moreLeft){
            digit_ = '4';
        }
        else if(!moreTop and moreLeft){
            digit_ = '6';
        }
        else{
            digit_ = '9';
        }
        

    }
    else if(euler_ == 1){
        //could be 1, 2, 3, 5, 7
        if(!moreLeft && !moreTop && vertsym_ < 0.75){
            if(vertcen_ > 0.55){
                digit_ = '1';
            }
            else{
                digit_ = '2';
            }
        }
        else if(moreTop && vertcen_ < 0.38 ){
            if(!moreLeft){
                digit_ = '7';
            }
            else if(horizcen_ < 0.6){
                digit_ = '7';
            }
        }
        else if(vertsym_ < 0.62){
            //sometimes 7 passes this, so check below
            if(vertcen_ < 0.4){
              digit_ = '7';
            }
            else{
              digit_ = '5';
            }
        }
        else if(moreLeft ){
            digit_ = '1';
        }
        else if(horizcen_ > 0.85){
            digit_ = '1';
        }
        else{
            digit_ = '3';
        }
    }


}

char DigitBlob::getClassification() const
{
    return digit_;
}

void DigitBlob::printClassificationResults() const
{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    cout << "Is top heavy? " << moreTop << endl;
    cout << "Is left heavy?" << moreLeft << endl;

    cout << "Vertical symmetry is: " << vertsym_ << endl;
    cout << "Vertical centroid is: " << vertcen_ << endl;
    cout << "Horizontal centroid is: " << horizcen_ << endl;
    cout << "Aspect ratio is: " << aspectrat_ << endl;
    
    //for spreadsheet
    // cout <<  moreTop << endl;
    // cout << moreLeft << endl;

    // cout <<  vertsym_ << endl;
    // cout << vertcen_ << endl;
    // cout <<  horizcen_ << endl;
    // cout << aspectrat_ << endl;

    cout << "Total rat " << totalRat << endl;
    cout << "top horiz symmetry: " << ths_ << endl;
    cout << "midrat: " << midRat << endl;
    


    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}

int DigitBlob::getHeight() const
{
    return h_;
}

int DigitBlob::getWidth() const
{
    return w_;
}

bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}

void DigitBlob::calc_bit_quads()
{
    for(int i=ul_.row-1; i < ul_.row + h_ ; i++){
        for(int j=ul_.col-1; j < ul_.col + w_ ; j++){
            //get four pixels
            int upl = img_[i][j];
            int upr = img_[i][j+1];
            int ll = img_[i+1][j];
            int lr = img_[i+1][j+1];
            //empty
            if(upl==255 && upr==255 && ll==255 && lr==255){
                bq0_ +=1;
            }
            //all the bq1s
            else if(upl==0 && upr==255 && ll==255 && lr==255){
                bq1_ +=1;
            }
            else if(upl==255 && upr==0 && ll==255 && lr==255){
                bq1_ +=1;
            }
            else if(upl==255 && upr==255 && ll==0 && lr==255){
                bq1_ +=1;
            }
            else if(upl==255 && upr==255 && ll==255 && lr==0){
                bq1_ +=1;
            }
            //all the bq2s
            else if(upl==0 && upr==0 && ll==255 && lr==255){
                bq2_ +=1;
            }
            else if(upl==255 && upr==0 && ll==0 && lr==255){
                bq2_ +=1;
            }
            else if(upl==255 && upr==255 && ll==0 && lr==0){
                bq2_ +=1;
            }
            else if(upl==0 && upr==255 && ll==255 && lr==0){
                bq2_ +=1;
            }
            //all the bq3s
            else if(upl==0 && upr==0 && ll==0 && lr==255){
                bq3_ +=1;
            }
            else if(upl==255 && upr==0 && ll==0 && lr==0){
                bq3_ +=1;
            }
            else if(upl==0 && upr==255 && ll==0 && lr==0){
                bq3_ +=1;
            }
            else if(upl==0 && upr==0 && ll==255 && lr==0){
                bq3_ +=1;
            }
            //all filled
            else if(upl==0 && upr==0 && ll==0 && lr==0){
                bq4_ +=1;
            }
            //all the bqds
            else if(upl==255 && upr==0 && ll==0 && lr==255){
                bqd_ +=1;
            }
            else if(upl==0 && upr==255 && ll==255 && lr==0){
                bqd_ +=1;
            }
        }
    }
}

void DigitBlob::calc_vertical_symmetry(){
    double sym = 0;
    int matchcount = 0;
    int totalcount = 0;
    for(int i = ul_.row; i < ul_.row + (h_/2); i++){
        for(int j = ul_.col; j < ul_.col + (w_); j++){
            totalcount+=1;
            if(img_[i][j] == img_[2*ul_.row + h_-(i+1)][j]){
                matchcount+=1;
            }
        }
    }
    sym = double(matchcount)/double(totalcount);
    vertsym_ = sym;
}

void DigitBlob::calc_centers_of_mass(){
    //vertical center of mass 
    int blackpixels = 0;
    int sum = 0;
    for(int i = ul_.row; i < (ul_.row + h_+1); i++){
        for(int j = ul_.col; j < (ul_.col + w_+1); j++){
            if(img_[i][j] == 0){
                blackpixels+=1;
                sum = sum + (i-ul_.row);
            }
        }
    }
    vertcen_ = (double(sum)/double(blackpixels))/(h_);

    //horizontal center of mass 
    for(int i = ul_.row; i < h_; i++){
        for(int j = ul_.col; j < w_; j++){
            if(img_[i][j] == 0){
                blackpixels+=1;
                sum = sum + j;
            }
        }
    }
    horizcen_ = (double(sum)/double(blackpixels))/(w_);
}


void DigitBlob::calc_aspect_ratio(){
    aspectrat_ = double(h_) / w_ ;
}

void DigitBlob::isTopHeavy(){
    int topblack = 0;
    int bottomblack = 0;
    int totalcount = 0;
    for(int i = ul_.row; i < ul_.row + (h_/2); i++){
        for(int j = ul_.col; j < ul_.col + (w_); j++){
            totalcount +=1;
            if(img_[i][j] == 0){
                topblack+=1;
            }
            if(img_[2*ul_.row + h_-(i+1)][j] == 0){
                bottomblack+=1;
            }
        }
    }
    if(topblack > bottomblack){
        moreTop = true;
    }
    else{
        moreTop = false;
    }
}

void DigitBlob::isLeftHeavy(){
    int leftblack = 0;
    int rightblack = 0;
    int totalcount = 0;
    for(int i = ul_.row; i < ul_.row + (h_); i++){
        for(int j = ul_.col; j < ul_.col + (w_/2); j++){
            totalcount +=1;
            if(img_[i][j] == 0){
                leftblack+=1;
            }
            if(img_[2*ul_.row + h_-(i+1)][2*ul_.col + w_ - (j+1)] == 0){
                rightblack+=1;
            }
        }
    }
    if(leftblack > rightblack){
        moreLeft = true;
    }
    else{
        moreLeft = false;
    }
    totalRat = (leftblack + rightblack)/double(totalcount);
}

void DigitBlob::topHoSym(){
    int matchcount = 0;
    int totalcount = 0;
    for(int i = ul_.row; i < ul_.row + (h_/2); i++){
        for(int j = ul_.col; j < ul_.col + (w_/2); j++){
            totalcount+=1;
            if(img_[i][j] == img_[i][2*ul_.col + w_-(j+1)]){
                matchcount+=1;
            }
        }
    }

    ths_ = double(matchcount)/double(totalcount);;
}

void DigitBlob::middleRatio(){
    int rstart = ul_.row + (0.25*h_);
    int rstop = ul_.row + (0.75*h_);
    int matchcount = 0;
    int totalcount = 0;
    
    for(int i = rstart; i <= rstop; i++){
        for(int j = ul_.col; j < ul_.col + w_; j++){
            totalcount+=1;
            if(img_[i][j] == 0){
                matchcount+=1;
            }
        }
    }
    midRat = double(matchcount)/totalcount;

}