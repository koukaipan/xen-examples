OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
SECTIONS
{
  . = 0x0;              /* Start of the output file */

  _text = .;            /* Text and read-only data */

  .text : {
    *(.text)
  } = 0x9090

  _etext = .;           /* End of text section */

  .rodata : {           /* Read only data section */
    *(.rodata) 
    *(.rodata.*) 
  }

  .data : {             /* Data */
    *(.data)
  }

  _edata = .;           /* End of data section */

}
