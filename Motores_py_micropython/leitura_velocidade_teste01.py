import machine
from machine import Pin, PWM, Timer

# Configurando pinos de acionamento do motor
MD_AIN1 = Pin(26, Pin.OUT)
MD_AIN2 = Pin(25, Pin.OUT)
ME_BIN1 = Pin(14, Pin.OUT)
ME_BIN2 = Pin(12, Pin.OUT)

PWMA_pin = Pin(33, Pin.OUT)
PWMB_pin = Pin(13, Pin.OUT)
STBY = Pin(27, Pin.OUT)

led = Pin(2, Pin.OUT)
# Iniciando Duty cicle como zero 
PWMAD = PWM(PWMA_pin,freq = 1000, duty = 0)
PWMBE = PWM(PWMB_pin,freq = 1000, duty = 0)

#Configurando pinos do Encoder
MD_OUTA = Pin(34, Pin.IN, Pin.PULL_UP)
MD_OUTB = Pin(39, Pin.IN, Pin.PULL_UP)
ME_OUTA = Pin(35, Pin.IN, Pin.PULL_UP)
ME_OUTB = Pin(32, Pin.IN, Pin.PULL_UP)

#Iniciando Timer's
tim0 = machine.Timer(0)
tim3 = machine.Timer(3)

MD_counter = 0
ME_counter = 0
Velocidade_RPM_D = 0
Velocidade_RPM_E = 0



def handle_interrupt_D (pin):
    global MD_counter

    if(MD_OUTA.value() == MD_OUTB.value()):
        MD_counter = MD_counter - 1
#         print ("Contador Direita:", MD_counter)
    else:
            MD_counter = MD_counter + 1
#             print ("Contador Direita:", MD_counter)
            
def handle_interrupt_E (pin):
    global ME_counter

    if(ME_OUTA.value() == ME_OUTB.value()):
        ME_counter = ME_counter - 1
#         print ("Contador Esquerda:", ME_counter)
    else:
            ME_counter = ME_counter + 1
#             print ("Contador Esquerda:", ME_counter)
            
def handle_callback_timer0 (timer):
    global Velocidade_RPM_D
    global Velocidade_RPM_E
    global MD_counter
    global ME_counter
    
    Velocidade_RPM_D = (((MD_counter * 10) / 900) * 60.0)
    Velocidade_RPM_E = (((ME_counter * 10) / 900) * 60.0)
    MD_counter = 0
    ME_counter = 0
    
    
            
tim0.init(period = 10000, mode = machine.Timer.PERIODIC, callback = handle_callback_timer0)
# tim3.init(period = 500, mode = machine.Timer.PERIODIC, callback = handle_callback_timer3)

MD_OUTA.irq(trigger= machine.Pin.IRQ_RISING, handler = handle_interrupt_D)
ME_OUTA.irq(trigger= machine.Pin.IRQ_RISING, handler = handle_interrupt_E)


    
    