// arm-linux-gcc -Wall -o led led.c

#include <stdio.h>
#include <fcntl.h>    // O_*
#include <unistd.h>   // close
#include <stdlib.h>   // atoi
#include <stdint.h>   // uint32_t
#include <sys/mman.h> // mmap

//#define TYPE int  // si on definit h comme int*, alors l'increment est en multiple de 4 !
#define TYPE char

int main(int argc, char** argv)
{ const int base_addr1= 0xF8000000;
  const int base_addr2= 0xe000a000;
  TYPE *h    = NULL; // int *h = NULL;
  int map_file        = 0;
  unsigned int page_addr,page_offset;
  unsigned page_size=sysconf(_SC_PAGESIZE);

  uint32_t value= 0x80; // 1 ou 128 pour LED orange ou rouge
  if (argc >= 2) value = atoi(argv[1]);
  printf("value = 0x%x\n",value);

  page_addr  = base_addr1 & (~(page_size-1));
  page_offset= base_addr1-page_addr;
  printf("page=0x%x size=0x%x offset=0x%x\n",page_addr,page_size,page_offset);

  // mmap the device into memory
  map_file = open("/dev/mem", O_RDWR | O_SYNC);
  h=mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,map_file,page_addr);
  if (h<0) {printf("mmap pointer: %x\n",(int)(h));return(-1);}

  // exemple baremetal pour debloquer acces SLCR registers
  // https://forums.xilinx.com/xlnx/attachments/xlnx/EDK/29780/1/helloworld.c
  printf("@      -> %x %x \n",(unsigned int)h,(unsigned int)(h+0x0c));
  printf("init   -> %x \n",(*(unsigned int*)(h+0x0c/sizeof(TYPE))));
  *(unsigned int*)(h+0x04/sizeof(TYPE))=0x767b; // lock
  printf("lock   -> %x\n",(*(unsigned int*)(h+0x0c/sizeof(TYPE))));
  *(unsigned int*)(h+0x08/sizeof(TYPE))=0xDF0D; // unlock
  printf("unlock -> %x\n",(*(unsigned int*)(h+0x0c/sizeof(TYPE))));

  // IL FAUT L'HORLOGE DE GPIO ! bit 22 de 0xF8000000+0x0000012C, sinon meme lecture echoue (0x00)
  // https://forums.xilinx.com/t5/Embedded-Linux/Zynq-mmap-GPIO/td-p/368601
  printf("ck -> %x\n",(*(unsigned int*)(h+0x12c/sizeof(TYPE))));
  *(unsigned int*)(h+0x12c/sizeof(TYPE))|=(1<<22);
  printf("ck -> %x\n",(*(unsigned int*)(h+0x12c/sizeof(TYPE))));

  page_addr = (base_addr2 & (~(page_size-1)));
  page_offset = base_addr2 - page_addr;
  printf("%x %x\n",page_addr,page_size);
  h=mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,map_file,page_addr);
  if (h<0) {printf("mmap pointer: %x\n",(int)(h));return(-1);}
  //write_reg(0xE000A000, 0x00000000, 0x7C020000); //MIO pin 9 value update
  //write_reg(0xE000A000, 0x00000204, 0x200);    //set direction of MIO9
  //write_reg(0xE000A000, 0x00000208, 0x200);    //output enable of MIO9
  //write_reg(0xE000A000, 0x00000040, 0x00000000); //output 0 on MIO9
  //data = read_reg(0xE000A000, 0x00000060);       //read data on MIO

  // https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf p.1347
  *(unsigned int*)(h+page_offset+0)=(value);     // GPIO programming sequence :p.386
  printf("hk: %x\n",*(unsigned int*)(h+page_offset));
  *(unsigned int*)(h+page_offset+0x204/sizeof(TYPE))=value;  // direction
  printf("hk+204: %x\n",*(unsigned int*)(h+page_offset+(0x204)/sizeof(TYPE)));
  *(unsigned int*)(h+page_offset+0x208/sizeof(TYPE))=value;  // output enable
  printf("hk+208: %x\n",*(unsigned int*)(h+page_offset+(0x208)/sizeof(TYPE)));
  *(unsigned int*)(h+page_offset+0x040/sizeof(TYPE))=value;  // output value
  printf("hk+40: %x\n",*(unsigned int*)(h+page_offset+0x40)/sizeof(TYPE));
  close(map_file);
  return 0;
}
