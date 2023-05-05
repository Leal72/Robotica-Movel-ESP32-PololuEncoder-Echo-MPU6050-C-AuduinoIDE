import machine

MD_AIN1 = machine.Pin(26,machine.Pin.OUT)
MD_AIN2 = machine.Pin(25,machine.Pin.OUT)
ME_BIN1 = machine.Pin(14,machine.Pin.OUT)
ME_BIN2 = machine.Pin(12,machine.Pin.OUT)
PWMA_pin = machine.Pin(33,machine.Pin.OUT)
PWMB_pin = machine.Pin(13,machine.Pin.OUT)
STBY = machine.Pin(27,machine.Pin.OUT)
led = machine.Pin(2,machine.Pin.OUT)

PWMAD = machine.PWM(PWMA_pin,freq = 1000, duty = 0)
PWMBE = machine.PWM(PWMB_pin,freq = 1000, duty = 0)

def direcao_velocidade (direcao, velocidade):
    if(direcao == 'Frente'):
        MD_AIN1(0)
        MD_AIN2(1)
        ME_BIN1(0)
        ME_BIN2(1)
        STBY(1)
        PWMAD.duty(velocidade)
        PWMBE.duty(velocidade)
        led(1)
        
    
