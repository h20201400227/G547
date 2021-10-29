#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/ioctl.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/slab.h>
#include<linux/random.h>

#include<linux/init.h>
#include<linux/kernel.h>


// #define DEVICE_NAME "imu_char"



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
 
 
int32_t value = 0;
int8_t temp = 0;
int16_t pres = 0;
int16_t GX = 0, GY = 0, GZ =0;
int16_t CX = 0, CY = 0, CZ =0;
int16_t AX = 0, AY = 0, AZ =0;
 



// holds device number

dev_t device_no = 0;

// cdev variable & file operation 

static struct class *dev_class;
static struct cdev imu_cdev;

// Functions 

static int      __init imu_char_init(void);
static void     __exit imu_char_exit(void);
static int      imu_open(struct inode *inode, struct file *file);
static int      imu_release(struct inode *inode, struct file *file);
static ssize_t  imu_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  imu_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     imu_ioctl(struct file *file, unsigned int cmd, unsigned long arg);



// File operations 

static struct file_operations fops =
{
        .read           = imu_read,
        .write          = imu_write,
        .open           = imu_open,
        .release        = imu_release,
        .unlocked_ioctl = imu_ioctl,
        .owner          = THIS_MODULE,
};


// Functions Implementation

// open function
static int imu_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "IMU device file opened\n");
        return 0;
}

// close function
static int imu_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "IMU device file closed\n");
        return 0;
}

// read function
static ssize_t imu_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Read function\n");
        return 0;
}


// Write Function

static ssize_t imu_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Write function\n");
        return len;
}



// IOCTL Functions

static long imu_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
         
                case WRITE_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Error during data write operation\n");
                        }
                        printk(KERN_INFO "Value = %d\n", value);
                        break;
                        
                case READ_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Error during data read operation\n");
                        }
                        break;
                        
                        
// BMP280 Barometric Pressure Sensor IOCTL Functions 
// BMP280 can measure temperature in the range 0 to 65C, and
// Pressure measurement range 300 to 1100hPa.

                case BMP280_PresVALUE:
                
                	do
                	{
                		get_random_bytes(&pres,2);
                		
                	}while((pres<300) || (pres >1100));
                	
                        if( copy_to_user((int16_t*) arg, &pres, sizeof(pres)) )
                        {
                                pr_err("Error during Pressue data read operation\n");
                        }
                        break;

                case BMP280_TempVALUE:
                	do
                	{
                		get_random_bytes(&temp,1);
                		
                	}while((temp<0) || (temp>65));
                	
                        if( copy_to_user((int8_t*) arg, &temp, sizeof(temp)) )
                        {
                                pr_err("Error during Temperature data read operation\n");
                        }
                        break;
                        
// MPU9255 Gyroscope Sensor IOCTL Functions 
                       
                case MPU9255_Gyro_XVALUE:
                	do
                	{
                		get_random_bytes(&GX,2);
                		
                	}while((GX< -250) || (GX> 250));
                	
                        if( copy_to_user((int16_t*) arg, &GX, sizeof(GX)) )
                        {
                                pr_err("Error during Gyroscope data read operation\n");
                        }
                        break;

                case MPU9255_Gyro_YVALUE:
                	do
                	{
                		get_random_bytes(&GY,2);
                		
                	}while((GY< -250) || (GY> 250));
                	
                        if( copy_to_user((int16_t*) arg, &GY, sizeof(GY)) )
                        {
                                pr_err("Error during Gyroscope data read operation\n");
                        }
                        break;

                case MPU9255_Gyro_ZVALUE:
                	do
                	{
                		get_random_bytes(&GZ,2);
                		
                	}while((GZ< -250) || (GZ> 250));
                	
                        if( copy_to_user((int16_t*) arg, &GZ, sizeof(GZ)) )
                        {
                                pr_err("Error during Gyroscope data read operation\n");
                        }
                        break;

// MPU9255 Accelerometer Sensor IOCTL Functions 
                       
                case MPU9255_Acce_XVALUE:
                	do
                	{
                		get_random_bytes(&AX,1);
                		
                	}while((AX< -16) || (AX> 16));
                	
                        if( copy_to_user((int16_t*) arg, &AX, sizeof(AX)) )
                        {
                                pr_err("Error during Accelerometer data read operation\n");
                        }
                        break;

                case MPU9255_Acce_YVALUE:
                	do
                	{
                		get_random_bytes(&AY,1);
                		
                	}while((AY< -16) || (AY> 16));
                	
                        if( copy_to_user((int16_t*) arg, &AY, sizeof(AY)) )
                        {
                                pr_err("Error during Accelerometer data read operation\n");
                        }
                        break;

                case MPU9255_Acce_ZVALUE:
                	do
                	{
                		get_random_bytes(&AZ,1);
                		
                	}while((AZ< -16) || (AZ> 16));
                	
                        if( copy_to_user((int16_t*) arg, &AZ, sizeof(AZ)) )
                        {
                                pr_err("Error during Accelerometer data read operation\n");
                        }
                        break;


// MPU9255 Magnetometer Sensor IOCTL Functions 
                       
                case MPU9255_Comp_XVALUE:
                	do
                	{
                		get_random_bytes(&CX,2);
                		
                	}while((CX< -4800) || (CX> 4800));
                	
                        if( copy_to_user((int16_t*) arg, &CX, sizeof(CX)) )
                        {
                                pr_err("Error during Magnetometer data read operation\n");
                        }
                        break;

                case MPU9255_Comp_YVALUE:
                	do
                	{
                		get_random_bytes(&CY,2);
                		
                	}while((CY< -4800) || (CY> 4800));
                	
                        if( copy_to_user((int16_t*) arg, &CY, sizeof(CY)) )
                        {
                                pr_err("Error during Magnetometer data read operation\n");
                        }
                        break;

                case MPU9255_Comp_ZVALUE:
                	do
                	{
                		get_random_bytes(&CZ,2);
                		
                	}while((CZ< -4800) || (CZ> 4800));
                	
                        if( copy_to_user((int16_t*) arg, &CZ, sizeof(CZ)) )
                        {
                                pr_err("Error during Magnetometer data read operation\n");
                        }
                        break;

                        
                default:
                        printk(KERN_INFO "Default State\n");
                        break;
        }
        return 0;
}



//Init Function 

static int __init imu_char_init(void){

// Dynamic allocation of device number
// Single device with base minor value 0

// alloc_chrdev_region(&device_no, 0, 1, "imusens");


        if((alloc_chrdev_region(&device_no, 0, 1, "imusens")) <0){
                pr_err("Failed to allocate major number\n");
                return -1;
        }

// Device number Information 

        printk(KERN_INFO "Major Number = %d Minor Number = %d \n",MAJOR(device_no), MINOR(device_no));
 
 
// For device registration,  initializing cdev structure with file operations

	cdev_init(&imu_cdev, &fops);
 
// Device Registration
// cdev_add(&imu_cdev, device_no, 1);

        if((cdev_add(&imu_cdev,device_no,1)) < 0){
            pr_err("Failed to add the device to the system\n");
            goto r_class;
        }
 
 
// Creating struct class

        if((dev_class = class_create(THIS_MODULE,"imu_class")) == NULL){
            pr_err("Failed to create the struct class\n");
            goto r_class;
        }
 
 
// Creating device
        if((device_create(dev_class,NULL,device_no,NULL,"imu_device")) == NULL){
            pr_err("Failed to create the Device 1\n");
            goto r_device;
        }
        
        printk(KERN_INFO "IMU Driver has been Inserted\n");
        return 0;


r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(device_no,1);
        return -1;




}



// Exit Function 

static void __exit imu_char_exit(void){

        device_destroy(dev_class,device_no);
        class_destroy(dev_class);
        
        cdev_del(&imu_cdev);
        unregister_chrdev_region(device_no, 1);
        
        printk(KERN_INFO "IMU Device Driver is Removed\n");


}




module_init(imu_char_init);
module_exit(imu_char_exit);



MODULE_AUTHOR("Chakradhar");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IMU Sensor Driver");
