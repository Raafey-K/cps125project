#include <stdio.h>
#include <math.h>

//Mean calculation function
float calMean(float data[], int size) {
    float sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += data[i];
    }
    return sum / size;
}

//Calculation standard deviation function
float calSD(float data[], int size) {
    float mean = calMean(data, size);
    float standardDev = 0.0;
    for (int i = 0; i < size; ++i) {
        standardDev += pow(data[i] - mean, 2);
    }
    return sqrt(standardDev / size);
}

//Slope calculation function
float calSlope(float x[], float y[], int size) {
    float xMean = calMean(x, size);
    float yMean = calMean(y, size);
    float numer = 0.0;
    float denom = 0.0;
    for (int i = 0; i < size; ++i) {
        numer += (x[i] - xMean) * (y[i] - yMean);
        denom += pow(x[i] - xMean, 2);
    }
    return numer / denom;
}

//read file data
void readDataFromFile(FILE *file, float lakeTemps[365][29]){
    rewind(file);
    for (int day = 0; day < 365; ++day){
        for (int year = 0; year < 29; ++year){
            fscanf(file, "%f,", &lakeTemps[day][year]);
        }
    }
}

//function for printing lake trends so that variables dont collide in intmain
void lakeprint(const char *lakeName, float lakeTemps[365][29]){
    
    // Variables to hold the results
    int increasingDays = 0, decreasingDays = 0, noTrendDays = 0;
    float maxSlope = -1.0e30, minSlope = 1.0e30;
    float averageSlope, slopeSD;
    float slopes[365];
    float years[29];
    for (int i = 0; i <29; i++){
        years[i] = 1995 + i;
    }

    // Calculate slopes and standard deviations for Lakes
    for (int day = 0; day < 365; ++day) {
        float dailyTemps[29];
        for (int year = 0; year < 29; ++year) {
            dailyTemps[year] = lakeTemps[day][year];
        }
        slopes[day] = calSlope(years, dailyTemps, 29);
        float sd = calSD(dailyTemps, 29);

        if (fabs(slopes[day]) >= 2 * sd) {
                if (slopes[day] > 0)
                    increasingDays++;
                else
                    decreasingDays++;
            } else {
                noTrendDays++;
            }

            if (slopes[day] > maxSlope)
                maxSlope = slopes[day];
            if (slopes[day] < minSlope)
                minSlope = slopes[day];
    }
        //calc avg and stddev of slopes
        averageSlope = calMean(slopes, 365);
        slopeSD = calSD(slopes,365);

        printf("\n%s:\n", lakeName);
        printf("Max Slope: %f\n", maxSlope);
        printf("Min Slope: %f\n", minSlope);
        printf("Increasing trend days: %d\n", increasingDays);
        printf("Decreasing trend days: %d\n", decreasingDays);
        printf("No trend days: %d\n", noTrendDays);
        printf("Average Slope: %f\n", averageSlope);
        printf("Slope Standard Deviation: %f\n", slopeSD);
        printf("Trend in Average Slope: %s\n", (fabs(averageSlope) >=2 * slopeSD) ? "Yes" : "No");
}

//Day to month-day conversion function
void convertDayToMonthDay(int dayOfYear, int *month, int *day) {
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    *month = 1;
    while (dayOfYear > daysInMonth[*month - 1]) {
        dayOfYear -= daysInMonth[*month - 1];
        (*month)++;
    }
    *day = dayOfYear;
}

void coldwarmDay(const char *lakeName, float lakeTemps[365][29]){

    // Calculate the average water temp for each day
    float dailyAverages[365];
    for (int day = 0; day < 365; ++day) {
        dailyAverages[day] = calMean(lakeTemps[day], 29);
    }

    // Find the coldest and warmest day
    int coldestDay = 0, warmestDay = 0;
    for (int day = 1; day < 365; ++day) {
        if (dailyAverages[day] < dailyAverages[coldestDay]) {
            coldestDay = day;
        }
        if (dailyAverages[day] > dailyAverages[warmestDay]) {
            warmestDay = day;
        }
    }

    // Convert day of year to month and day
    int coldestMonth, coldestDayOfMonth, warmestMonth, warmestDayOfMonth;
    convertDayToMonthDay(coldestDay + 1, &coldestMonth, &coldestDayOfMonth);
    convertDayToMonthDay(warmestDay + 1, &warmestMonth, &warmestDayOfMonth);

    printf("\n%s: \n", lakeName);
    printf("Coldest Day: %d/%d\n", coldestMonth, coldestDayOfMonth);
    printf("Warmest Day: %d/%d\n", warmestMonth, warmestDayOfMonth);

}

void summerAvg(const char *lakeName, float lakeTemps[365][29]){
    // Calculate summer averages for all 6 lakes
    float summerTemps[29 * (265 - 172 + 1)]; // All summer days for all years
    int index = 0;
    for (int day = 172; day <= 265; ++day) {
        for (int year = 0; year < 29; ++year) {
            summerTemps[index++] = lakeTemps[day][year];
        }
    }
    float summerAverages = calMean(summerTemps, index);

    printf("\nSummer Averages for lake %s: %f", lakeName, summerAverages);
}

// Function to calculate slope and trend (for swimdays)
void calTrend(const char *lakeName, float years[], float data[], int size) {
    float slope = calSlope(years, data, size);
    float sd = calSD(data, size);

    // Determine if there is a trend
    printf("The slope of swim days:%f\n", slope);
    printf("Standard Deviation of swim days: %f\n", sd);
    printf("Trend in Swim Days for Lake %s: %s\n", lakeName, (fabs(slope) >= 2 * sd) ? "Yes" : "No");
}

// Calculate swim days for lake stclair
void swimDays(const char *lakeName, float lakeTemps[365][29]){
    int swimDays[29] = {0};
    for (int year = 0; year < 29; ++year) {
        for (int day = 0; day < 365; ++day) {
            if (lakeTemps[day][year] > 20) {
                swimDays[year]++;
            }
        }
    }
    // Output the swim days
    for (int year = 0; year < 29; ++year) {
        printf("Year %d: %d\n", 1995 + year, swimDays[year]);
    }

    //slope, sd, trend
    float swimDaysFloat[29];
    for (int i = 0; i < 29; ++i) {
        swimDaysFloat[i] = (float) swimDays[i];
    }
    float years[29];
    for (int i = 0; i < 29; ++i) {
        years[i] = 1995 + i;
    }
    calTrend(lakeName, years, swimDaysFloat, 29);
}

//Main function and arrays
int main() {
    
FILE *ofile = fopen("all_year_glsea_avg_o_C.csv", "r");
FILE *efile = fopen("all_year_glsea_avg_e_C.csv", "r");
FILE *hfile = fopen("all_year_glsea_avg_h_C.csv", "r");
FILE *mfile = fopen("all_year_glsea_avg_m_C.csv", "r");
FILE *sfile = fopen("all_year_glsea_avg_s_C.csv", "r");
FILE *cfile = fopen("all_year_glsea_avg_c_C.csv", "r");

// Check if all files were opened successfully
if (ofile == NULL || efile == NULL || hfile == NULL || mfile == NULL || sfile == NULL || cfile == NULL){
    perror("Error opening file\n");
    return 1;
}

// Arrays to hold data for each lake
float lakeOntarioTemps[365][29];
float lakeErieTemps[365][29];
float lakeHuronTemps[365][29];
float lakeMichiganTemps[365][29];
float lakeSuperiorTemps[365][29];
float lakeClairTemps[365][29];

// Read data for each lake
readDataFromFile(ofile, lakeOntarioTemps);
readDataFromFile(efile, lakeErieTemps);
readDataFromFile(hfile, lakeHuronTemps);
readDataFromFile(mfile, lakeMichiganTemps);
readDataFromFile(sfile, lakeSuperiorTemps);
readDataFromFile(cfile, lakeClairTemps);

// Close all files
fclose(ofile);
fclose(efile);
fclose(hfile);
fclose(mfile);
fclose(sfile);
fclose(cfile);

printf("Lake temperature data: \n");
lakeprint("Lake Ontario", lakeOntarioTemps);
lakeprint("Lake Erie", lakeErieTemps);
lakeprint("Lake Huron", lakeHuronTemps);
lakeprint("Lake Michigan", lakeMichiganTemps);
lakeprint("Lake Superior", lakeSuperiorTemps);
lakeprint("Lake St. Clair", lakeClairTemps);

printf("\n--------------------\n\nColdest and warmest days for each lake: \n");
coldwarmDay("Lake Ontario", lakeOntarioTemps);
coldwarmDay("Lake Erie", lakeErieTemps);
coldwarmDay("Lake Huron", lakeHuronTemps);
coldwarmDay("Lake Michigan", lakeMichiganTemps);
coldwarmDay("Lake Superior", lakeSuperiorTemps);
coldwarmDay("Lake St. Clair", lakeClairTemps);

printf("\n--------------------\n\nAverage temperature for each lake during summers: \n");
summerAvg("Lake Ontario", lakeOntarioTemps);
summerAvg("Lake Erie", lakeErieTemps);
summerAvg("Lake Huron", lakeHuronTemps);
summerAvg("Lake Michigan", lakeMichiganTemps);
summerAvg("Lake Superior", lakeSuperiorTemps);
summerAvg("Lake St. Clair", lakeClairTemps);

printf("\n\n--------------------\nSwim days in lake St. Clair: \n\n");
swimDays("Lake St. Clair", lakeClairTemps);

return 0;
}
