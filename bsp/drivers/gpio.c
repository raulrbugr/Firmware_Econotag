/*
 * Sistemas operativos empotrados
 * Driver para el GPIO del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del gpio del MC1322x
 */
typedef struct
{
	/* ESTA ESTRUCTURA SE DEFINIRÁ EN LA PRÁCTICA 7 */
	uint32_t PAD_DIR[2]; 			/* Selección de dirección del pad */
	uint32_t DATA[2]; 				/* Datos */
	uint32_t PU_EN[2]; 				/* Habilitación de pull-up/pull_down */
	uint32_t FUNC_SEL[4]; 			/* Selección de la función conectada al GPIO */
	uint32_t DATA_SEL[2]; 			/* Selección del origen de los datos de entrada */
	uint32_t PAD_PU_SEL[2]; 		/* Selección entre pull-up y pull-down */
	uint32_t PAD_HYST_EN[2]; 		/* Habilitación de histéresis */
	uint32_t PAD_KEEP[2]; 			/* Habilitación de mantener el último estado del pin */
	uint32_t DATA_SET[2]; 			/* Activa bits en GPIO_DATA */
	uint32_t DATA_RESET[2]; 		/* Desactiva bits en GPIO_DATA */
	uint32_t DIR_SET[2]; 			/* Activa bits en GPIO_PAD_DIR */
	uint32_t DIR_RESET[2]; 			/* Desactiva bits en GPIO_PAD_DIR */
} gpio_regs_t;

static volatile gpio_regs_t* const gpio_regs = GPIO_BASE;

/*****************************************************************************/

/**
 * Fija la dirección los pines seleccionados en la máscara como de entrada
 *
 * @param 	port 	Puerto
 * @param 	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_input (gpio_port_t port, uint32_t mask)
{
    if(port >= gpio_port_max)
        return gpio_invalid_parameter;
    gpio_regs->DIR_RESET[port] = mask;
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección los pines seleccionados en la máscara como de salida
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_output (gpio_port_t port, uint32_t mask)
{
    if(port >= gpio_port_max)
        return gpio_invalid_parameter;
    gpio_regs->DIR_SET[port] = mask;
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de entrada
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_input (gpio_pin_t pin)
{
    if(pin >= gpio_pin_max)
        return gpio_invalid_parameter;
    //Dividimos el numero de pin por 32 para saber el puerto y hacemos
    //el módulo 32 para que todos los pines queden en el rango [0,31]
    gpio_regs->DIR_RESET[pin >> 5] = ( 1 << (pin & 0x1f) );
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de salida
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_output (gpio_pin_t pin)
{
    if(pin >= gpio_pin_max)
        return gpio_invalid_parameter;
    //Dividimos el numero de pin por 32 para saber el puerto y hacemos
    //el módulo 32 para que todos los pines queden en el rango [0,31]
    gpio_regs->DIR_SET[pin >> 5] = ( 1 << (pin & 0x1f) );
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe unos en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port (gpio_port_t port, uint32_t mask)
{
    if(port >= gpio_port_max)
        return gpio_invalid_parameter;
    gpio_regs->DATA_SET[port] = mask;
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe ceros en los pines seleccionados en la máscara
 *
 * @param	port 	Puerto
 * @param	mask 	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_port (gpio_port_t port, uint32_t mask)
{
    if(port >= gpio_port_max)
        return gpio_invalid_parameter;
    gpio_regs->DATA_RESET[port] = mask;
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un uno en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin (gpio_pin_t pin)
{
    if(pin >= gpio_pin_max)
        return gpio_invalid_parameter;
    //Dividimos el numero de pin por 32 para saber el puerto y hacemos
    //el módulo 32 para que todos los pines queden en el rango [0,31]
    gpio_regs->DATA_SET[pin >> 5] = ( 1 << (pin & 0x1f) );
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Escribe un cero en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_pin (gpio_pin_t pin)
{
    if(pin >= gpio_pin_max)
        return gpio_invalid_parameter;
    //Dividimos el numero de pin por 32 para saber el puerto y hacemos
    //el módulo 32 para que todos los pines queden en el rango [0,31]
    gpio_regs->DATA_RESET[pin >> 5] = ( 1 << (pin & 0x1f) );
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Lee el valor de los pines de un puerto
 *
 * @param	port	  Puerto
 * @param	port_data Valor de los pines del puerto
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_port (gpio_port_t port, uint32_t *port_data)
{
    if(port >= gpio_port_max)
        return gpio_invalid_parameter;
    *port_data = gpio_regs->DATA[port];
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Lee el valor del pin indicado
 *
 * @param	pin	  Número de pin
 * @param       pin_data  Cero si el pin está a cero, distinto de cero en otro caso
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_pin (gpio_pin_t pin, uint32_t *pin_data)
{
    if(pin >= gpio_pin_max || pin < gpio_pin_0)
        return gpio_invalid_parameter;
    *pin_data = gpio_regs->DATA[pin >> 5] & ( 1 << (pin & 0x1f) );
    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija los pines seleccionados a una función
 *
 * @param	port 	Puerto
 * @param	func	Función
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_func (gpio_port_t port, gpio_func_t func, uint32_t mask)
{
    if(port >= gpio_port_max || func >= gpio_func_max)
        return gpio_invalid_parameter;

    uint32_t i, pin, reg, offset;

    for (i=0; i<32; ++i){
        if (mask & (1<<i)){	    							// Si el pin se modifica
            pin = i + (port << 5);
            reg = pin >> 4;                 				//se divide el pin entre 16 para saber que registro le corresponde
            offset = (pin & 0xf) << 1;      				//(pin % 16) * 2
            gpio_regs->FUNC_SEL[reg] &= (~(3 << offset));   //bit clear
            gpio_regs->FUNC_SEL[reg] |= (func << offset);   //se escribe la funcion
        }
    }

    return gpio_no_error;
}

/*****************************************************************************/

/**
 * Fija el pin seleccionado a una función
 *
 * @param	pin 	Pin
 * @param	func	Función
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_func (gpio_pin_t pin, gpio_func_t func)
{
    if(pin >= gpio_pin_max || func >= gpio_func_max)
        return gpio_invalid_parameter;

    uint32_t  reg = pin >> 4,   //se divide el pin entre 16 para saber que registro le corresponde
              offset = (pin & 0xf) << 1; //(pin % 16) * 2

    gpio_regs->FUNC_SEL[reg] &= ~( 3 << offset );   //bit clear
    gpio_regs->FUNC_SEL[reg] |= ( func << offset ); //se escribe la funcion
    return gpio_no_error;
}

/*****************************************************************************/
