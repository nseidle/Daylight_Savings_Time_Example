/*
 Given a date, UTC time and local offset determine the local time including daylight savings time
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 28th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 I've had a few projects (Wimp Weather Station, GPS Clock, etc) that needed local time corrected with daylight savings.
 Here is a cleaned up 

 */

int local_hour_offset = 7; //Boulder Colorado (MST) is -7 hours from GMT

void setup()
{
  Serial.begin(9600);
  Serial.println("DST Testing");
}

void loop()
{
  Serial.println("2014");
  Serial.print("March 1. Should return 5: ");
  Serial.println(calcLocalHour(2014, 3, 1, 12, local_hour_offset)); //March 1, 2014. 12UTC DST

  Serial.print("March 9. Should return 6: ");
  Serial.println(calcLocalHour(2014, 3, 9, 12, local_hour_offset)); //Spring forward March 9, 2014. 12UTC DST

  Serial.print("Nov 1. Should return 6: ");
  Serial.println(calcLocalHour(2014, 11, 1, 12, local_hour_offset)); //Nov 1, 2014. 12UTC

  Serial.print("Nov 2. Should return 5: ");
  Serial.println(calcLocalHour(2014, 11, 2, 12, local_hour_offset)); //Fall back Nov 2, 2014. 12UTC DST

  Serial.print("Nov 26. Should return 5: ");
  Serial.println(calcLocalHour(2014, 11, 26, 12, local_hour_offset)); //Nov 26, 2014. 12UTC

  //2015
  Serial.println();
  Serial.println("2015");
  Serial.print("March 7. Should return 5: ");
  Serial.println(calcLocalHour(2015, 3, 7, 12, local_hour_offset));

  Serial.print("March 8. Should return 6: ");
  Serial.println(calcLocalHour(2015, 3, 8, 12, local_hour_offset));

  Serial.print("Oct 30. Should return 6: ");
  Serial.println(calcLocalHour(2015, 10, 30, 12, local_hour_offset));

  Serial.print("Nov 1. Should return 5: ");
  Serial.println(calcLocalHour(2015, 11, 1, 12, local_hour_offset));

  while(1);
}

//Given a year/month/day/current UTC/local offset give me local time
byte calcLocalHour(int year, byte month, byte day, byte hour, byte offset)
{
  //Since 2007 DST starts on the second Sunday in March and ends the first Sunday of November
  //Let's just assume it's going to be this way for awhile (silly US government!)
  //Example from: http://stackoverflow.com/questions/5590429/calculating-daylight-savings-time-from-only-date

  byte DoW = day_of_week(year, month, day); //Get the day of the week. 0 = Sunday, 6 = Saturday
  int previousSunday = day - DoW;

  boolean dst = false; //Assume we're not in DST
  if(month > 3 && month < 11) dst = true; //DST is happening!

  //In March, we are DST if our previous Sunday was on or after the 8th.
  if (month == 3)
  {
    if(previousSunday >= 8) dst = true; 
  } 
  //In November we must be before the first Sunday to be dst.
  //That means the previous Sunday must be before the 1st.
  if(month == 11)
  {
    if(previousSunday <= 0) dst = true;
  }
  
  if(dst == true) hour++;

  //Convert UTC hours to local current time using local_hour
  if(hour < local_hour_offset)
    hour += 24; //Add 24 hours before subtracting local offset
  hour -= local_hour_offset;
  if(hour > 12) hour -= 12; //Get rid of military time
  
  /*
  Serial.print("Hour: ");
  Serial.println(hour);

  Serial.print("Day of week: ");
  if(DoW == 0) Serial.println("Sunday");
  if(DoW == 1) Serial.println("Monday");
  if(DoW == 2) Serial.println("Tuesday");
  if(DoW == 3) Serial.println("Wednesday");
  if(DoW == 4) Serial.println("Thursday");
  if(DoW == 5) Serial.println("Friday!");
  if(DoW == 6) Serial.println("Saturday");
  */

  return(hour);
}

//Given the current year/month/day
//Returns 0 (Sunday) through 6 (Saturday) for the day of the week
//From: http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week
//This function assumes the month from the caller is 1-12
char day_of_week(int year, int month, int day)
{
  //Devised by Tomohiko Sakamoto in 1993, it is accurate for any Gregorian date:
  static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4  };
  year -= month < 3;
  return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}



