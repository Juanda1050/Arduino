bool estado = false;
bool estadoAcelerador = false;
const int pulsador = 2;
const int acelerador = 3; 
int retardo = 3000;
bool acceso = false;

void setup()
{  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(2, INPUT); 
  pinMode(3, INPUT); 
}

void loop()
{
  estado = digitalRead(pulsador);
  
  if(estado)
  {
    retardo = 3000;
    acceso = true;
  }
  
  if(acceso == true)
  {
    digitalWrite(13, HIGH);
    delay(retardo);
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(13,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(13,LOW);
        digitalWrite(12,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo); 
    }
    else
    {
      if(acceso)
      {
        digitalWrite(13,LOW);
        digitalWrite(12,HIGH);
        delay(retardo); 
      }
	}
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(12,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(12,LOW);
        digitalWrite(11,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(12,LOW);
        digitalWrite(11,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(11,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(11,LOW);
        digitalWrite(10,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(11,LOW);
        digitalWrite(10,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(10,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(10,LOW);
        digitalWrite(7,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(10,LOW);
        digitalWrite(7,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(7,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(7,LOW);
        digitalWrite(6,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(7,LOW);
        digitalWrite(6,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(6,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	retardo = retardo - (retardo*0.1);
        digitalWrite(6,LOW);
        digitalWrite(7,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        retardo = retardo - (retardo*0.1);
        digitalWrite(6,LOW);
        digitalWrite(7,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(7,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(7,LOW);
        digitalWrite(10,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(7,LOW);
        digitalWrite(10,HIGH);
        delay(retardo);
      }
      
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(10,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
     	digitalWrite(10,LOW);
        digitalWrite(11,HIGH);
      	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(10,LOW);
        digitalWrite(11,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(11,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      	digitalWrite(11,LOW);
        digitalWrite(12,HIGH);
     	retardo = retardo - (retardo*0.1);
        delay(retardo);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(11,LOW);
        digitalWrite(12,HIGH);
        delay(retardo);
      }
    }
    
    estado = digitalRead(pulsador);
    estadoAcelerador = digitalRead(acelerador);
    if(estado)
    {
      acceso = false;
      digitalWrite(12,LOW);
      delay(1000);
    }
    else if(estadoAcelerador)
    {
      digitalWrite(12,LOW);
      retardo = retardo - (retardo*0.1);
    }
    else
    {
      if(acceso)
      {
        digitalWrite(12,LOW);
      }
    }
    
    if(retardo<=10)
    {
      retardo = 3000;
    }
    else
    {
      retardo = retardo - (retardo*0.1);
    }
  }
}
  