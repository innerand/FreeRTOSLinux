/*
 * Linker script extension of the FreeRTOS Linux Port
 *
 * Add `-T freertos_linux.ld` to the linker flags
 *
 * Allows reinitialization of static FreeRTOS variables at runtime (if they are
 * stored at this section.)
 *
 */

SECTIONS
{
    .free_rtos : {
        . = ALIGN(8);
        PROVIDE(__free_rtos_start__ = .);
        *(free_rtos)
        . = ALIGN(8);
        PROVIDE(__free_rtos_stop__ = .);
    }
}
INSERT AFTER .bss
