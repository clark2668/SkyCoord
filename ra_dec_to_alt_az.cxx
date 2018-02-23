#include <ctime>
#include "time.h"
#include "TMath.h"

//simple c++ program to convert RA and DEC to local ALT and AZ
//Inputs:
//RA and DEC in J2000 frame
//time will be in UTC time
//need to provide LON and LAT
//LON is -180 -> 180, with zero as prime meridian and west as negative
//LAT is -90 to 90, with zero as equator and north as positive and south as negative

//Outputs:
//Altitude and Azimuth in degrees

//Limitations:
//Only good for years 2009 -> 2019 (only work with leap years 2012 and 2016)


//you should run this as a root macro
//open root, and type ".L ra_dec_to_alt_az.cxx"
//then you can call "main (blah, blah, blah, blah, blah)"

//function declarations
int days_till_start_of_month(int month, int year);
double days_since_2000(int year);
double rerange(double LST);

using namespace std;

void main(int UTC, double RA, double DEC, double LON, double LAT){
	
	//need to decode the UTC time
	int unixtime = UTC;
	time_t dummy_time = unixtime;
	tm *time = gmtime(&dummy_time);
	int year = time->tm_year+1900;
	int month = time->tm_mon+1;
	int day = time->tm_mday;
	int hour = time->tm_hour;
	int min = time->tm_min;
	int sec = time->tm_sec;
	
	//do error checking
	//make sure they didn't ask for an unsupported year
	if(year<2009 || year>2019){//I didn't code to account for these!
		cout<<"This code only works for 2009->2019! Your year requested is: "<<year<<endl;
		cout<<"Exiting...>"<<endl;
		return -1;
	}
	
	//now, to compute the time since J2000
	int days_till_start = days_till_start_of_month(month, year);
	double days_to_year = days_since_2000(year);
	double part_of_day = (double(hour) + (double(min)/60.) + (double(min)/60./60.))/24.; //convert this to a fraction of 24 hours
	double days_since_j2000 = part_of_day + double(days_till_start) + days_to_year + double(day);
	double hours_in_day = (double(hour) + (double(min)/60.));
	
	//cout<<"Part of day "<<part_of_day<<endl;
	//cout<<"Days to year "<<days_to_year<<endl;
	//cout<<"Days since J2000 "<<days_since_j2000<<endl;
	//cout<<"LON "<<LON<<endl;
	
	//now to compute the local siderial time
	double LST = 100.46 + (0.985647 * days_since_j2000) + LON + (15.*hours_in_day);
	//cout<<"LST "<<LST<<endl;
	
	//now, need to re-range LST to be between 0 and 360
	LST=rerange(LST);
	//cout<<"Rerange LST "<<LST<<endl;
	
	//finally to compute the hour angle
	double HA = LST - RA;
	
	//need to rerange the HA
	rerange(HA);
	
	//now to compute a bunch of the stuff we're going to need in the alt az computation
	double sin_dec = TMath::Sin(DEC*TMath::DegToRad());
	double sin_lat = TMath::Sin(LAT*TMath::DegToRad());
	double cos_dec = TMath::Cos(DEC*TMath::DegToRad());
	double cos_lat = TMath::Cos(LAT*TMath::DegToRad());
	double cos_ha = TMath::Cos(HA*TMath::DegToRad());
	
	//compute the sine of altitude
	double sin_alt = (sin_dec * sin_lat) + (cos_dec * cos_lat * cos_ha);
	//now get the actual altitude
	double alt = TMath::ASin(sin_alt)*TMath::RadToDeg();
	
	//compute the cosine of A
	double cos_a = (sin_dec - (sin_alt * sin_lat)) / TMath::Cos(alt*TMath::DegToRad()) / cos_lat;
	//now get the actual A
	double a = TMath::ACos(cos_a)*TMath::RadToDeg();
	
	//now, check to see if we need to re-range the A to get azimuth
	double az =0.;
	if(TMath::Sin(HA*TMath::DegToRad())<0) az = a; //if sin(HA)<0 then az = A
	else az = 360. - a; //otherwise Az = 360 - A
	
	//print the result!
	cout<<"(Az, Alt) in deg: ("<<az<<" , "<<alt<<")"<<endl;
}

//this will be a recursive function to make LST between 0 and 360
double rerange(double LST){	
	if(LST<0.){ //if it's less 0
		while(LST<0.) LST+=360; //while it's less 0, keep adding 360.
	}
	if(LST>360.){ //if it's great 0
		while(LST>360.) LST-=360; //while it's greater 0, keep subtracting 360.
	}
	return LST;
}

int days_till_start_of_month(int month, int year){
	int result = 0;
	switch(month){
		case 1: {result = 0; break;}
		case 2: {result = 31; break;}
		case 3: {result = 59; break;}
		case 4: {result = 90; break;}
		case 5: {result = 120; break;}
		case 6: {result = 151; break;}
		case 7: {result = 181; break;}
		case 8: {result = 212; break;}
		case 9: {result = 243; break;}
		case 10: {result = 273; break;}
		case 11: {result = 304; break;}
		case 12: {result = 334; break;}
	}
	//if it's a leap year, we need to add one day
	if((year==2012 || year==2016) && month>2) result++;
	
	//return result
	return result;
}

double days_since_2000(int year){
	double result = 0;
	switch(year){
		case 2009: {result = 3286.5; break;}
		case 2010: {result = 3651.5; break;}
		case 2011: {result = 4016.5; break;}
		case 2012: {result = 4381.5; break;}
		case 2013: {result = 4747.5; break;}
		case 2014: {result = 5112.5; break;}
		case 2015: {result = 5477.5; break;}
		case 2016: {result = 5842.5; break;}
		case 2017: {result = 6208.5; break;}
		case 2018: {result = 6573.5; break;}
		case 2019: {result = 6938.5; break;}
	}
	
	//return result
	return result;
}