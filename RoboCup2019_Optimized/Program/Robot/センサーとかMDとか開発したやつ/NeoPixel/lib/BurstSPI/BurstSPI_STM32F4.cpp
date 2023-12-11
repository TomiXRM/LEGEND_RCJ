#if defined(TARGET_STM32F4)
#include "BurstSPI.h"
 
void BurstSPI::fastWrite(int data) {
    
    SPI_TypeDef *spi = (SPI_TypeDef *)(_spi.spi);
    // Check if data is transmitted
    while ((spi->SR & SPI_SR_TXE) == 0);
    spi->DR = data;
}
    
void BurstSPI::clearRX( void ) {
    //Check if the RX buffer is busy
    SPI_TypeDef *spi = (SPI_TypeDef *)(_spi.spi);
    //While busy, keep checking
    while (spi->SR & SPI_SR_BSY){   
        // Check RX buffer readable
        while ((spi->SR & SPI_SR_RXNE) == 0);
        int dummy = spi->DR;
    }
}
#endif

