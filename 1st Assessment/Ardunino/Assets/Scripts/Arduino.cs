using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO.Ports;
//This tells the project, that I am using an Ardunio kit and are using open ports

public class Arduino : MonoBehaviour {

    public GameObject playerOne;
    public GameObject playerTwo;
    //These two open a public varaiable in the inspector, which allows me to drag the two player paddles on

    public int commPort = 0;
    //This allows me to change the port number that the arduino is connected too in the inspector 
    public float scaleSize = 10f;


    private SerialPort serial;


	
	void Start ()
    {
        serial = new SerialPort("\\\\.\\COM" + commPort, 9600);

        serial.ReadTimeout = 50;

        serial.Open();
	}
	
	
	void Update ()
    {
        WriteToArduino("P");
        string values = ReadFromArduino();

        if(values != null)
        {
            string[] splitValues = values.Split('-');

            if(splitValues.Length == 2)
            {
                positionPlayers(splitValues);
            }
        }
	}

    void positionPlayers(string[] values)
    {
        if(playerOne != null)
        {
            float scale = Remap(int.Parse(values[0]), 0f, 1023f, 0f, scaleSize);
            Vector3 newPosition = new Vector3(playerOne.transform.position.x,
                scale, playerOne.transform.position.z);

            playerOne.transform.position = newPosition;
        }

        if (playerTwo != null)
        {
            float scale = Remap(int.Parse(values[1]), 0f, 1023f, 0f, scaleSize);
            Vector3 newPosition2 = new Vector3(playerTwo.transform.position.x,
                scale, playerTwo.transform.position.z);

            playerTwo.transform.position = newPosition2;
        }

    }

    public void WriteToArduino(string msg)
    {
        serial.WriteLine(msg);
        serial.BaseStream.Flush();
    }

    string ReadFromArduino()
    {
        serial.ReadTimeout = 50;

        try
        {
            return serial.ReadLine();
        }
        catch(TimeoutException e)
        {
            return null;
        }
    }

    private void OnDestroy()
    {
        serial.Close();
    }

   float Remap(float value, float from1, float to1, float from2, float to2)
   {
        return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
   }
}
