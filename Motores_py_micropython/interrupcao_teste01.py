import machine


led = machine.Pin(2,machine.Pin.OUT)
sw = machine.Pin(0,machine.Pin.IN)
tim0 = machine.Timer(0)
counter = 0

def handle_interrupt (pin):
    global counter
    counter = counter + 1
    print ("Contador :", counter)
    
def handle_callback (timer):
    global Pulsos
    global counter
    led.value(not led.value())
    Pulsos = counter *10
    counter = 0
    print ("Pulsos :", Pulsos)

    
# tim0.init(period = 100, mode = machine.Timer.PERIODIC, callback = lambda t: led.value(not led.value()))
tim0.init(period = 10000, mode = machine.Timer.PERIODIC, callback = handle_callback)

          
sw.irq(trigger=machine.Pin.IRQ_FALLING, handler = handle_interrupt)
