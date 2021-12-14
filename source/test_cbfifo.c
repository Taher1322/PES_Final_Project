/*****************************************************************************
* Copyright (C) 2021 by Taher Ujjainwala
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Taher Ujjainwala and the University of Colorado are not liable for
* any misuse of this material.
*
**************************************************************************/


/*************************
 *
 *
 *
 *    File name   : test_cbfifo.c - test the cbfifo implementation
 *    Description : This file tests the Circular Buffer functions
 *
 *    Author Howdy Pierce, howdy.pierce@colorado.edu
 *    I would like to thank Howdy for this amazing test cases for CBFIFO
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2
 *
 *    Date  : 11/09/2021
 *
 *
 */


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_cbfifo.h"
#include "cbfifo.h"
#include "uart.h"

static void test_cbfifo_transmit()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf[1024];
  const int cap = cbfifo_capacity(TXD);

  // asserts in following 2 lines -- this is not testing the student,
  // it's validating that the test is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  assert(cap == 256 || cap == 255);

  assert(cbfifo_length(TXD) == 0);
  assert(cbfifo_dequeue(buf, cap, TXD) == 0);
  assert(cbfifo_dequeue(buf, 1, TXD) == 0);

  // enqueue 10 bytes, then dequeue same amt
  assert(cbfifo_enqueue(str, 10, TXD) == 10);
  assert(cbfifo_length(TXD) == 10);
  assert(cbfifo_dequeue(buf, 10, TXD) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(cbfifo_length(TXD) == 0);


  // enqueue 20 bytes;  dequeue 5, then another 20
  assert(cbfifo_enqueue(str, 20, TXD) == 20);
  assert(cbfifo_length(TXD) == 20);
  assert(cbfifo_dequeue(buf, 5, TXD) == 5);
  assert(cbfifo_length(TXD) == 15);
  assert(cbfifo_dequeue(buf+5, 20, TXD) == 15);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str, 20) == 0);

  // fill buffer and then read it back out
  assert(cbfifo_enqueue(str, cap, TXD) == cap);
  assert(cbfifo_length(TXD) == cap);
  assert(cbfifo_enqueue(str, 1, TXD) == 0);
  assert(cbfifo_dequeue(buf, cap, TXD) == cap);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // Add 20 bytes and pull out 18
  assert(cbfifo_enqueue(str, 20, TXD) == 20);
  assert(cbfifo_length(TXD) == 20);
  assert(cbfifo_dequeue(buf, 18, TXD) == 18);
  assert(cbfifo_length(TXD) == 2);
  assert(strncmp(buf, str, 18) == 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    assert(cbfifo_enqueue(str+i*chunk_size, chunk_size, TXD) == chunk_size);
    assert(cbfifo_length(TXD) == (i+1)*chunk_size + 2);
  }
  assert(cbfifo_length(TXD) == 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  assert(cbfifo_dequeue(buf, 2, TXD) == 2);
  assert(strncmp(buf, str+18, 2) == 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    assert(cbfifo_dequeue(buf+i, 1, TXD) == 1);
    assert(cbfifo_length(TXD) == chunk_size*4 - i - 1);
  }
  assert(strncmp(buf, str, chunk_size*4) == 0);

  // write more than capacity
  assert(cbfifo_enqueue(str, 65, TXD) == 65);
  assert(cbfifo_enqueue(str+65, cap, TXD) == cap-65);
  assert(cbfifo_length(TXD) == cap);
  assert(cbfifo_dequeue(buf, cap, TXD) == cap);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // write zero bytes
  assert(cbfifo_enqueue(str, 0, TXD) == 0);
  assert(cbfifo_length(TXD) == 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  assert(cbfifo_enqueue(str, 32, TXD) == 32);  // advance so that read < write
  assert(cbfifo_length(TXD) == 32);
  assert(cbfifo_dequeue(buf, 16, TXD) == 16);
  assert(cbfifo_length(TXD) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, 32, TXD) == 32);  // (1)
  assert(cbfifo_length(TXD) == 48);
  assert(cbfifo_enqueue(str+64, cap-64, TXD) == cap-64);  // (2)
  assert(cbfifo_length(TXD) == cap-16);
  assert(cbfifo_dequeue(buf+16, cap-16, TXD) == cap-16);
  assert(strncmp(buf, str, cap) == 0);

  assert(cbfifo_enqueue(str, 32, TXD) == 32);  // advance so that read < write
  assert(cbfifo_length(TXD) == 32);
  assert(cbfifo_dequeue(buf, 16, TXD) == 16);
  assert(cbfifo_length(TXD) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap-20, TXD) == cap-20);  // (3)
  assert(cbfifo_length(TXD) == cap-4);
  assert(cbfifo_dequeue(buf, cap-8, TXD) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(cbfifo_length(TXD) == 4);
  assert(cbfifo_dequeue(buf, 8, TXD) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(cbfifo_length(TXD) == 0);

  assert(cbfifo_enqueue(str, 49, TXD) == 49);  // advance so that read < write
  assert(cbfifo_length(TXD) == 49);
  assert(cbfifo_dequeue(buf, 16, TXD) == 16);
  assert(cbfifo_length(TXD) == 33);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+49, cap-33, TXD) == cap-33);  // (4)
  assert(cbfifo_length(TXD) == cap);
  assert(cbfifo_dequeue(buf, cap, TXD) == cap);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(str, 32, TXD) == 32);  // advance so that read < write
  assert(cbfifo_length(TXD) == 32);
  assert(cbfifo_dequeue(buf, 16, TXD) == 16);
  assert(cbfifo_length(TXD) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap, TXD) == cap-16);  // (5)
  assert(cbfifo_dequeue(buf, 1, TXD) == 1);
  assert(cbfifo_length(TXD) == cap-1);
  assert(cbfifo_dequeue(buf+1, cap-1, TXD) == cap-1);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  assert(cbfifo_enqueue(str, cap-4, TXD) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(TXD) == cap-4);
  assert(cbfifo_dequeue(buf, 32, TXD) == 32);
  rpos += 32;
  assert(cbfifo_length(TXD) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, TXD) == 12);
  wpos += 12;
  assert(cbfifo_length(TXD) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 16, TXD) == 16);  // (6)
  assert(cbfifo_length(TXD) == cap-8);
  assert(cbfifo_dequeue(buf, cap, TXD) == cap-8);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str+rpos, cap-8) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, TXD) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(TXD) == cap-4);
  assert(cbfifo_dequeue(buf, 32, TXD) == 32);
  rpos += 32;
  assert(cbfifo_length(TXD) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, TXD) == 12);
  wpos += 12;
  assert(cbfifo_length(TXD) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 24, TXD) == 24);  // (7)
  assert(cbfifo_length(TXD) == cap);
  assert(cbfifo_dequeue(buf, cap, TXD) == cap);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, TXD) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(TXD) == cap-4);
  assert(cbfifo_dequeue(buf, 32, TXD) == 32);
  rpos += 32;
  assert(cbfifo_length(TXD) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, TXD) == 12);
  wpos += 12;
  assert(cbfifo_length(TXD) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 64, TXD) == 24);  // (8)
  assert(cbfifo_length(TXD) == cap);
  assert(cbfifo_dequeue(buf, cap, TXD) == cap);
  assert(cbfifo_length(TXD) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  cbfifo_reset(TXD);

  //printf("\n\r %s: passed all test cases\n\r", __FUNCTION__);
}

static void test_cbfifo_receive()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf[1024];
  const int cap = cbfifo_capacity(RXD);

  // asserts in following 2 lines -- this is not testing the student,
  // it's validating that the test is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  assert(cap == 256 || cap == 255);

  assert(cbfifo_length(RXD) == 0);
  assert(cbfifo_dequeue(buf, cap, RXD) == 0);
  assert(cbfifo_dequeue(buf, 1, RXD) == 0);

  // enqueue 10 bytes, then dequeue same amt
  assert(cbfifo_enqueue(str, 10, RXD) == 10);
  assert(cbfifo_length(RXD) == 10);
  assert(cbfifo_dequeue(buf, 10, RXD) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(cbfifo_length(RXD) == 0);


  // enqueue 20 bytes;  dequeue 5, then another 20
  assert(cbfifo_enqueue(str, 20, RXD) == 20);
  assert(cbfifo_length(RXD) == 20);
  assert(cbfifo_dequeue(buf, 5, RXD) == 5);
  assert(cbfifo_length(RXD) == 15);
  assert(cbfifo_dequeue(buf+5, 20, RXD) == 15);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str, 20) == 0);

  // fill buffer and then read it back out
  assert(cbfifo_enqueue(str, cap, RXD) == cap);
  assert(cbfifo_length(RXD) == cap);
  assert(cbfifo_enqueue(str, 1, RXD) == 0);
  assert(cbfifo_dequeue(buf, cap, RXD) == cap);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // Add 20 bytes and pull out 18
  assert(cbfifo_enqueue(str, 20, RXD) == 20);
  assert(cbfifo_length(RXD) == 20);
  assert(cbfifo_dequeue(buf, 18, RXD) == 18);
  assert(cbfifo_length(RXD) == 2);
  assert(strncmp(buf, str, 18) == 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    assert(cbfifo_enqueue(str+i*chunk_size, chunk_size, RXD) == chunk_size);
    assert(cbfifo_length(RXD) == (i+1)*chunk_size + 2);
  }
  assert(cbfifo_length(RXD) == 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  assert(cbfifo_dequeue(buf, 2, RXD) == 2);
  assert(strncmp(buf, str+18, 2) == 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    assert(cbfifo_dequeue(buf+i, 1, RXD) == 1);
    assert(cbfifo_length(RXD) == chunk_size*4 - i - 1);
  }
  assert(strncmp(buf, str, chunk_size*4) == 0);

  // write more than capacity
  assert(cbfifo_enqueue(str, 65, RXD) == 65);
  assert(cbfifo_enqueue(str+65, cap, RXD) == cap-65);
  assert(cbfifo_length(RXD) == cap);
  assert(cbfifo_dequeue(buf, cap, RXD) == cap);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // write zero bytes
  assert(cbfifo_enqueue(str, 0, RXD) == 0);
  assert(cbfifo_length(RXD) == 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  assert(cbfifo_enqueue(str, 32, RXD) == 32);  // advance so that read < write
  assert(cbfifo_length(RXD) == 32);
  assert(cbfifo_dequeue(buf, 16, RXD) == 16);
  assert(cbfifo_length(RXD) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, 32, RXD) == 32);  // (1)
  assert(cbfifo_length(RXD) == 48);
  assert(cbfifo_enqueue(str+64, cap-64, RXD) == cap-64);  // (2)
  assert(cbfifo_length(RXD) == cap-16);
  assert(cbfifo_dequeue(buf+16, cap-16, RXD) == cap-16);
  assert(strncmp(buf, str, cap) == 0);

  assert(cbfifo_enqueue(str, 32, RXD) == 32);  // advance so that read < write
  assert(cbfifo_length(RXD) == 32);
  assert(cbfifo_dequeue(buf, 16, RXD) == 16);
  assert(cbfifo_length(RXD) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap-20, RXD) == cap-20);  // (3)
  assert(cbfifo_length(RXD) == cap-4);
  assert(cbfifo_dequeue(buf, cap-8, RXD) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(cbfifo_length(RXD) == 4);
  assert(cbfifo_dequeue(buf, 8, RXD) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(cbfifo_length(RXD) == 0);

  assert(cbfifo_enqueue(str, 49, RXD) == 49);  // advance so that read < write
  assert(cbfifo_length(RXD) == 49);
  assert(cbfifo_dequeue(buf, 16, RXD) == 16);
  assert(cbfifo_length(RXD) == 33);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+49, cap-33, RXD) == cap-33);  // (4)
  assert(cbfifo_length(RXD) == cap);
  assert(cbfifo_dequeue(buf, cap, RXD) == cap);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(str, 32, RXD) == 32);  // advance so that read < write
  assert(cbfifo_length(RXD) == 32);
  assert(cbfifo_dequeue(buf, 16, RXD) == 16);
  assert(cbfifo_length(RXD) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap, RXD) == cap-16);  // (5)
  assert(cbfifo_dequeue(buf, 1, RXD) == 1);
  assert(cbfifo_length(RXD) == cap-1);
  assert(cbfifo_dequeue(buf+1, cap-1, RXD) == cap-1);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  assert(cbfifo_enqueue(str, cap-4, RXD) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(RXD) == cap-4);
  assert(cbfifo_dequeue(buf, 32, RXD) == 32);
  rpos += 32;
  assert(cbfifo_length(RXD) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, RXD) == 12);
  wpos += 12;
  assert(cbfifo_length(RXD) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 16, RXD) == 16);  // (6)
  assert(cbfifo_length(RXD) == cap-8);
  assert(cbfifo_dequeue(buf, cap, RXD) == cap-8);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str+rpos, cap-8) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, RXD) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(RXD) == cap-4);
  assert(cbfifo_dequeue(buf, 32, RXD) == 32);
  rpos += 32;
  assert(cbfifo_length(RXD) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, RXD) == 12);
  wpos += 12;
  assert(cbfifo_length(RXD) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 24, RXD) == 24);  // (7)
  assert(cbfifo_length(RXD) == cap);
  assert(cbfifo_dequeue(buf, cap, RXD) == cap);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, RXD) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(RXD) == cap-4);
  assert(cbfifo_dequeue(buf, 32, RXD) == 32);
  rpos += 32;
  assert(cbfifo_length(RXD) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, RXD) == 12);
  wpos += 12;
  assert(cbfifo_length(RXD) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 64, RXD) == 24);  // (8)
  assert(cbfifo_length(RXD) == cap);
  assert(cbfifo_dequeue(buf, cap, RXD) == cap);
  assert(cbfifo_length(RXD) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);
  cbfifo_reset(RXD);
  //printf("\n\r %s: passed all test cases\n\r", __FUNCTION__);
}

void test_cbfifo()
{
	test_cbfifo_transmit();
	test_cbfifo_receive();
}


