/*
 * File: k60n512_flash.ld
 *
 * Copyright (c) 2013, 0xc0170
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 *
 */

ENTRY(Reset_Handler)

MEMORY
{
  VECTORS       (rx) : ORIGIN = 0x00000000, LENGTH = 0x410
  FLASH         (rx) : ORIGIN = 0x00000800, LENGTH = 0x80000 - 0x800
  SRAM_U       (rwx) : ORIGIN = 0x1FFF0000, LENGTH = 0x10000
  SRAM_L       (rwx) : ORIGIN = 0x20000000, LENGTH = 0x10000
}

SECTIONS
{
  .vectortable :
  {
    __vector_table = .;
    . = ALIGN(4);
    KEEP(*(.vectors))
    . = ALIGN(4);
  } > VECTORS

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  .ARM.extab :
  {
     *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH

  __exidx_end = .;

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > FLASH

  __etext = .;

  .data : AT(__etext)
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > SRAM_U

  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > SRAM_U

  .heap (COPY):
  {
    __end__ = .;
    end = __end__;
    *(.heap*)
    __HeapLimit = .;
  } > SRAM_U

  .stack_dummy (COPY):
  {
    *(.stack*)
  } > SRAM_U

  __StackTop = ORIGIN(SRAM_U) + LENGTH(SRAM_U);
  __StackLimit = __StackTop - SIZEOF(.stack_dummy);
  PROVIDE(__stack = __StackTop);
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

  .ARM.attributes 0 :
  {
    *(.ARM.attributes)
  }
}
