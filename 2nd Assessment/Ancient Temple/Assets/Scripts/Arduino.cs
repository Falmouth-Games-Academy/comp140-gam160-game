using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO.Ports;

public class Arduino : MonoBehaviour
{
    public GameObject player;

    private bool B1 = true;
    private bool B2 = true;

    private int pressed;



    public int commPort = 0;
    //This allows me to change the port number that the arduino is connected too in the inspector 


    private SerialPort serial;


    // Use this for initialization
    void Start ()
    {
        serial = new SerialPort("\\\\.\\COM" + commPort, 9600);

        serial.ReadTimeout = 100;

        serial.Open();
    }
	
	// Update is called once per frame
	void Update ()
    {
        WriteToArduino("F");
        string values = ReadFromArduino();

        if (values != null)
        {
            string[] splitValues = values.Split('-');
           


            if (splitValues.Length == 6)
            {
                positionPlayers(splitValues);
            }
        }
    }

    void positionPlayers(string[] values)
    {
        if (player != null)
        {
            if (int.Parse(values[0]) > 20)
            {
                player.GetComponent<PlayerController>().MovePlayer(-5f);
            }

            else if (int.Parse(values[1]) > 20)
            {
                player.GetComponent<PlayerController>().MovePlayer(5f);
            }
            else
            {
                player.GetComponent<PlayerController>().MovePlayer(0f);
            }
            if (int.Parse(values[2]) > 511)
            {
                B1 = true;
            }
            else
            {
                B1 = false;
            }
            if(B1 != B2)
            {
                if (player.GetComponent<CharacterController>().isGrounded)
                {
                    player.GetComponent<PlayerController>().Jump();
                    B2 = B1;
                }
            }
            if (int.Parse(values[3]) > 0)
            {
                ;
            }
            if (int.Parse(values[4]) > 0)
            {
                ;
            }
            if (int.Parse(values[5]) > 0)
            {
                ;
            }
        }

    }

    public void WriteToArduino(string msg)
    {
        serial.WriteLine(msg);
        serial.BaseStream.Flush();
    }

    string ReadFromArduino()
    {
        serial.ReadTimeout = 100;

        try
        {
            return serial.ReadLine();
        }
        catch (TimeoutException e)
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
