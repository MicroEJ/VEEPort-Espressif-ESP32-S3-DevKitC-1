/*
 * C
 *
 * Copyright 2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */


#include "esp32s3/rom/uart.h"
#include "esp_rom_gpio.h"
#include "uart_switch.h"
#include "soc/gpio_periph.h"
#include "hal/gpio_hal.h"
// When using external UART, GPIO 4 and 39 are used.
#define CONFIG_CONSOLE_UART_TX_GPIO 4
#define CONFIG_CONSOLE_UART_RX_GPIO 39

// This function should be called early in the bootloader
// In bootloader_init.c:uart_console_configure
void uart_switch(void) {
	const int uart_num = CONFIG_ESP_CONSOLE_UART_NUM;

	// Do nothing if traces should not be redirected
	// This uses a link constante that is usually set by the MicroEJ application linker.
	if (!MEJ_UART_EXT()) {
		return;
	}

	// Some constants to make the following code less upper-case
	const int uart_tx_gpio = CONFIG_CONSOLE_UART_TX_GPIO;
	const int uart_rx_gpio = CONFIG_CONSOLE_UART_RX_GPIO;
	// Switch to the new UART (this just changes UART number used for
	// ets_printf in ROM code).
	uart_tx_switch(uart_num);
	// If console is attached to UART1 or if non-default pins are used,
	// need to reconfigure pins using GPIO matrix
	if (uart_num != 0 || uart_tx_gpio != 43 || uart_rx_gpio != 44) {
		// Change pin mode for GPIO43/44 from UART to GPIO
		gpio_hal_iomux_func_sel(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_GPIO44);
		gpio_hal_iomux_func_sel(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_GPIO43);
		// Route GPIO signals to/from pins
		// (arrays should be optimized away by the compiler)
		const uint32_t tx_idx_list[3] = { U0TXD_OUT_IDX, U1TXD_OUT_IDX, U2TXD_OUT_IDX };
		const uint32_t rx_idx_list[3] = { U0RXD_IN_IDX, U1RXD_IN_IDX, U2RXD_IN_IDX };
		const uint32_t tx_idx = tx_idx_list[uart_num];
		const uint32_t rx_idx = rx_idx_list[uart_num];
		gpio_hal_iomux_func_sel(GPIO_PIN_MUX_REG[uart_rx_gpio], FUNC_U0RXD_GPIO44);
		PIN_INPUT_ENABLE(GPIO_PIN_MUX_REG[uart_rx_gpio]);

		esp_rom_gpio_pad_pullup_only(uart_rx_gpio);
		
		esp_rom_gpio_connect_out_signal(uart_tx_gpio, tx_idx, 0, 0);
		esp_rom_gpio_connect_in_signal(uart_rx_gpio, rx_idx, 0);

	}
}
