#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/ioctl.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

 
#define BMP280_PresVALUE _IOR('a','a',int16_t*)
#define BMP280_TempVALUE _IOR('a','b',int8_t*)

#define MPU9255_Gyro_XVALUE _IOR('a','c',int16_t*)
#define MPU9255_Gyro_YVALUE _IOR('a','d',int16_t*)
#define MPU9255_Gyro_ZVALUE _IOR('a','e',int16_t*)

#define MPU9255_Acce_XVALUE _IOR('a','f',int16_t*)
#define MPU9255_Acce_YVALUE _IOR('a','g',int16_t*)
#define MPU9255_Acce_ZVALUE _IOR('a','h',int16_t*)

#define MPU9255_Comp_XVALUE  _IOR('a','i',int16_t*)
#define MPU9255_Comp_YVALUE  _IOR('a','j',int16_t*)
#define MPU9255_Comp_ZVALUE  _IOR('a','k',int16_t*)

#define WRITE_VALUE _IOW('a','l',int32_t*)
#define READ_VALUE _IOR('a','m',int32_t*)
 
int main()
{
        int fd;
        int32_t value, number;
        int8_t temp;
        int16_t pres;
        int16_t GX,GY,GZ;
        int16_t AX,AY,AZ;
        int16_t CX,CY,CZ;
        int option = 0;
 
        printf("\nIMU Driver is ready \n");
        fd = open("/dev/imu_device", O_RDWR);
        
        if(fd < 0) {
                printf("Failed to open the file\n");
                return 0;
        }
        
	printf("Getting the Pressure Value from the driver\n");
        ioctl(fd, BMP280_PresVALUE, (int16_t*) &pres);
        printf("  Choose a option \n 1. hPa[Hectopascal] \n 2. Bar \n");
        scanf("%d",&option);
        if(option == 2){
        	printf("Measured Pressure Value is %d Bar \n\n", (pres/1000));
        }
        else{
        	printf("Measured Pressure Value is %d hPa \n\n", pres);
        }
        
 	printf("Getting the Temperature Value from the driver\n");
        ioctl(fd, BMP280_TempVALUE, (int8_t*) &temp);
        printf("  Choose a option \n 1. Celsius \n 2. Kelvin \n 3. Farenheit \n");
        scanf("%d",&option);
        if(option == 2){
        	printf("Measured Temperature Value is %d Kelvin \n\n", (temp+273));
        }
        else if(option == 3){
        	printf("Measured Temperature Value is %d Farenheit \n\n", (((temp*9)/5)+32));
        }
        else{
        	printf("Measured Temperature Value is %d Celsius \n\n", temp);
        }

 	printf("Getting the Gyroscope Values from the driver\n");
        ioctl(fd, MPU9255_Gyro_XVALUE, (int16_t*) &GX);
        ioctl(fd, MPU9255_Gyro_YVALUE, (int16_t*) &GY);
        ioctl(fd, MPU9255_Gyro_ZVALUE, (int16_t*) &GZ);
        printf(" Measured Gyroscope Values are \n X- %d, Y- %d, Z- %d in Degrees per second\n\n",GX, GY, GZ);

 	printf("Getting the Accelerometer Values from the driver\n");
        ioctl(fd, MPU9255_Acce_XVALUE, (int16_t*) &AX);
        ioctl(fd, MPU9255_Acce_YVALUE, (int16_t*) &AY);
        ioctl(fd, MPU9255_Acce_ZVALUE, (int16_t*) &AZ);
        printf(" Measured Accelerometer Values are \n X- %d, Y- %d, Z- %d in g force\n\n",AX, AY, AZ);

 	printf("Getting the Magnetometer Values from the driver\n");
        ioctl(fd, MPU9255_Comp_XVALUE, (int16_t*) &CX);
        ioctl(fd, MPU9255_Comp_YVALUE, (int16_t*) &CY);
        ioctl(fd, MPU9255_Comp_ZVALUE, (int16_t*) &CZ);
        printf(" Measured Magnetometer Values are \n X- %d, Y- %d, Z- %d in microtesla\n\n",CX, CY, CZ);
       
        printf("Writing a value to the device \n Enter a value\n");
        scanf("%d",&number);
        ioctl(fd, WRITE_VALUE, (int32_t*) &number);
        printf(" Value has been written\n\n");
 
        printf("Reading the value from the device\n");
        ioctl(fd, READ_VALUE, (int32_t*) &value);
        printf(" Read value is %d\n", value);
 
        printf(" Closing the driver file\n");
        close(fd);
}
