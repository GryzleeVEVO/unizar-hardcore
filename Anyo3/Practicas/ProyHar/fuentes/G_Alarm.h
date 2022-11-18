#ifndef G_ALARM_H
#define G_ALARM_H

/*
    Programa una alarma al recibir un mensaje Set_Alarma. 
    
    Los 8 primeros bitsdel mensaje representan el mensaje a generar; los 
    siguientes 24 el retardo en milisegundoos.

    Solo puede haber 8 alarmas, una por cada tipo de mensaje. Si se recibe un 
    mensaje con los mismos 8 primeros bits que alguna de las alaramas 
    programadas, reinicia la cuenta con el retardo dado. Si el retardo es 0,
    se cancela la alarma
*/
void alarm_programar_alarma(uint32_t msg);

/*
    Actualiza la cuenta al recibir un evento Get_Alarma, reduciendo la alarma
    por el periodo del temporizador. Si una alarma llega a 0, se encola un 
    mensaje del tipo dado
*/
void alarm_refrescar();

#endif // G_ALARM_H
