#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../cuppu86/src/cpu.h"
#include "../cuppu86/src/extras.h"
#include "../cuppu86/src/memory.h"
#include "../cuppu86/src/flagops.h"
#include "../cuppu86/src/inc_dec.h"
#include "../cuppu86/src/add_sub.h"

cpu* c;
int k;

void w8(u8 instr) {
   cpu_write_u8_at(c, base_offset(c->cs, c->ip + k), instr);
   k++;
}

void w16(u16 instr) {
   cpu_write_u16_at(c, base_offset(c->cs, c->ip + k), instr);
   k+=2;
}

void flagPrint(){
	unsigned i;
   printf("  _  _  _  _  O  D  I  T  S  Z  _  A  _  P  _  C\n");
	for (i = 1 << 15; i > 0; i = i / 2)
		(c->flags & i) ? printf("  1") : printf("  0");
   printf("\n");
}

void clrscr()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);
}

void instr_det(u32 instr) {
   printf("\nOpcode: [%x]", instr);
   switch (instr) {
   case 0x00 : printf("     add r/m8, r8");break;
   case 0x01 : printf("     add r/m16, r16");break;
   case 0x02 : printf("     add r8, r/m8");break;
   case 0x03 : printf("     add r16, r/m16");break;
   case 0x04 : printf("     add AL, imm8");break;
   case 0x05 : printf("     add AX, imm16");break;
   case 0x06 : printf("     push ES");break;
   case 0x07 : printf("     pop ES");break;
   case 0x08 : printf("     or r/m8, r8");break;
   case 0x09 : printf("     or r/m16, r16");break;
   case 0x0A : printf("     or r8, r/m8");break;
   case 0x0B : printf("     or r16, r/m16");break;
   case 0x0C : printf("     or AL, imm8");break;
   case 0x0D : printf("     or AX, imm16");break;
   case 0x0E : printf("     push CS");break;
   /*case 0x0F : to be updated*/

   case 0x10 : printf("     adc r/m8, r8");break;
   case 0x11 : printf("     adc r/m16, r16");break;
   case 0x12 : printf("     adc r8, r/m8");break;
   case 0x13 : printf("     adc r16, r/m16");break;
   case 0x14 : printf("     adc AL, imm8");break;
   case 0x15 : printf("     adc AX, imm16");break;
   case 0x16 : printf("     push SS");break;
   case 0x17 : printf("     pop SS");break;
   case 0x18 : printf("     sbb r/m8, r8");break;
   case 0x19 : printf("     sbb r/m16, r16");break;
   case 0x1A : printf("     sbb r8, r/m8");break;
   case 0x1B : printf("     sbb r16, r/m16");break;
   case 0x1C : printf("     sbb AL, imm8");break;
   case 0x1D : printf("     sbb AX, imm16");break;
   case 0x1E : printf("     push DS");break;
   case 0x1F : printf("     pop DS");break;

   case 0x20 : printf("     and r/m8, r8");break;
   case 0x21 : printf("     and r/m16, r16");break;
   case 0x22 : printf("     and r8, r/m8");break;
   case 0x23 : printf("     and r16, r/m16");break;
   case 0x24 : printf("     and AL, imm8");break;
   case 0x25 : printf("     and AX, imm16");break;
   /*case 0x26 : to be updated*/
   case 0x27 : printf("     daa AL");break;
   case 0x28 : printf("     sub r/m8, r8");break;
   case 0x29 : printf("     sub r/m16, r16");break;
   case 0x2A : printf("     sub r8, r/m8");break;
   case 0x2B : printf("     sub r16, r/m16");break;
   case 0x2C : printf("     sub AL, imm8");break;
   case 0x2D : printf("     sub AX, imm16");break;
   /*case 0x2E : to be updated*/
   case 0x2F : printf("     das AL");break;
   
   case 0x30 : printf("     xor r/m8, r8");break;
   case 0x31 : printf("     xor r/m16, r16");break;
   case 0x32 : printf("     xor r8, r/m8");break;
   case 0x33 : printf("     xor r16, r/m16");break;
   case 0x34 : printf("     xor AL, imm8");break;
   case 0x35 : printf("     xor AX, imm16");break;
   /*case 0x36 : to be updated*/
   case 0x37 : printf("     aaa AL, AH");break;
   case 0x38 : printf("     cmp r/m8, r8");break;
   case 0x39 : printf("     cmp r/m16, r16");break;
   case 0x3A : printf("     cmp r8, r/m8");break;
   case 0x3B : printf("     cmp r16, r/m16");break;
   case 0x3C : printf("     cmp AL, imm8");break;
   case 0x3D : printf("     cmp AX, imm16");break;
   /*case 0x3E : to be updated*/
   case 0x3F : printf("     aas AL, AH");break;

   /*case 0x40 - 0x5F* : to be updated*/

   case 0x60 : printf("     pusha");break;
   case 0x61 : printf("     popa");break;
   case 0x62 : printf("     bound");break;
   case 0x63 : printf("     arpl");break;
   /*case 0x64-0x67 : to be updated*/
   case 0x68 : printf("     push imm16");break;
   case 0x69 : printf("     imul r16, imm16");break;
   case 0x6A : printf("     push imm8");break;
   case 0x6B : printf("     imul r16, imm8");break;
   /*case 0x6C-0x7F : to be updated*/ 

   case 0x88 : printf("     mov r8 to r/m8\n");break;
   case 0x89 : printf("     mov r16 to r16\n");break;
   case 0x8a : printf("     mov r/m8 to r8\n");break;
   case 0x8b : printf("     mov r/m16 to r16\n");break;
   case 0x8c : printf("     mov Sreg to r/m16\n");break;
   case 0x8e : printf("     mov r/m16 to Sreg\n");break;
    
   
   case 0xb0 : printf("     mov imm8 to r8 (Low)\n");break;
   case 0xb4 : printf("     mov imm8 to r8 (high)\n");break;
   case 0xb9 : printf("     mov imm16 to r16\n");break;

   case 0xfe : printf("     inc/dec r/m8\n");break;
   case 0xff : printf("     inc/dec r/m16\n");break;
 
   }
}

char alternate(cpu*c,char ch) {
   char dum;
   int a,b;
   switch(ch) {
      case 'd':scanf("%c",&dum);
               if(dum == 10) {
                     cpu_dump_mem(c, base_offset(c->cs, c->ip), base_offset(c->cs, c->ip+40));
                  } /*add functionality to dump in range*/
               break;
      
      case 'h':scanf("%x ",&a);
               scanf("%x",&b);
               printf("%x %x\n",add16(c,a,b,0),sub16(c,a,b,0));
               break;

      case 'r':cpu_dump(c);
               break;

      
   }
}

void test_move_binary(cpu *c) {
   u32 instr;
   char ch;
   printf("\n-");
   scanf("%c",&ch);

   while(ch != 'q') {
      if(ch == 't') {
         clrscr();
         instr = cpu_fetch(c);
         instr_det(instr);
         cpu_exec(c, instr);
         cpu_dump(c);
         flagPrint();
         }
      else alternate(c,ch);
      printf("-");
      scanf("\n%c",&ch);
   }
   clrscr();
   printf("fetch/execute test done.\n------------------------------------------\n");
}

int main(int argc, char* argv[]) {
   u32 addr;
   u8 mem[MAX_MEMORY];
   u8 iop[MAX_MEMORY];
   k = 0;

   c = (cpu*) malloc(sizeof(cpu));
   cpu_init(c);
   cpu_setmem(c, mem,iop);

   w8(0xb0);
   w8(0xff);
   w8(0xb4);
   w8(0xaa);
   w8(0xb9);
   w16(0x22da);
   w8(0x88);
   w8(0x2f);
   w8(0x89);
   w8(0x06);
   w16(0x5000);
   w8(0x8b);
   w8(0xc3);
   w8(0x8a);
   w8(0xf1);
   w8(0x8b);
   w8(0x1e);
   w16(0x0100);
   w8(0x8e);
   w8(0xd2);
   w8(0x8c);
   w8(0xd0);
   w8(0xff);
   w8(0xca);
   w8(0xff);
   w8(0xac);
   w8(0xff);
   w8(0xc4);
   w8(0xff);
   w8(0x37);
   w8(0xfe); //fe
   w8(0xc0);
   w8(0xff);
   w8(0x07);

   test_move_binary(c);
   return 0;
}



