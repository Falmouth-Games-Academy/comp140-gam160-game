using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System;
using UnityEngine.UI;

public class Arduino : MonoBehaviour {
    //The code used has come from Andy's workshop and has been modified
    
    [SerializeField]
    private GameObject Shaker;
    [SerializeField]
    private GameObject ShakerMove;
    [SerializeField]
    private int commPort = 0;

    [SerializeField]
    private Text x;
    [SerializeField]
    private Text y;
    [SerializeField]
    private Text z;
    [SerializeField]
    private Text xAcc;
    [SerializeField]
    private Text yAcc;
    [SerializeField]
    private Text zAcc;
    [SerializeField]
    private Text arrayLength;

    public Vector3 shakerPos;
    public string val;

    private Vector3 rotationVector;
    private Vector3 currentAcc;
    private Vector3 previousAcc;
    private Vector3 previousPos;
    private Vector3 direction;
    float angelY;
    float angelX;
    float angelZ;

    float posX;
    float posY;
    float posZ;
    float X;
    float Y;
    float Z;
    float accX;
    float accY;
    float accZ;
    float num;

    Vector3 previousDistance;
    Vector3 distance;

    List<float> movementList = new List<float>();

    private SerialPort serial;
    string[] splitValues;
    private int valueID = 0;
    public string values;

    // Use this for initialization
    void Start () {
        //The varialbe serial is set to listen out for information on the set port the arduino is broadcasting from
        //While also telling unity the rate of bits per second the arduino will be sending and recieving.
        serial = new SerialPort("\\\\.\\COM" + commPort, 9600);
        //The serial variable is set to time out and open a new port if Unity doesn't detect any communication for about 50 milliseconds
        serial.ReadTimeout = 50;
        //Unity will also open the specified port above
        serial.Open();
        WriteToArduio("r");
        WriteToArduio("p");
    }
	
	// Update is called once per frame
	void Update () {
        //Unity will send a request for values from the potentiometers every frame
        WriteToArduio("p");
        values = ReadFromArduino();
        

        //If the values exist then the information will be split into two
        if (values != null)
        {
            //The value will be split into two to get the different positions
            splitValues = values.Split(',');

            /*for (int i = 0; i < splitValues.Length; i++)
            {
                valueID = i;
            }
            
            foreach (string value in splitValues)
            {
                Debug.Log(value);
            }
            */
            
            //The function player position is run if values exists
            PositionPlayer(splitValues);


            
        }

        
    }

    //The method below will send the data stored in the msg parameter 
    //snd remove previous information so that it can access new incoming information
    public void WriteToArduio(string msg)
    {
        serial.WriteLine(msg);
        serial.BaseStream.Flush();
    }

    //The method below will read the information from the arduino
    string ReadFromArduino()
    {
        //If Unity doesn't resieve any thing after 50 milliseconds then the function will return nothing
        //Other wise the function will resturnthe newest line it reads from the arduino
        serial.ReadTimeout = 50;
        try
        {
            return serial.ReadLine();
        }
        catch (TimeoutException)
        {
            return null;
        }     
    }

    //The void will get and set the position / rotation of the shaker depending on the angle of the gryoscope/accelerometer
    void PositionPlayer(string[] values)
    {
        if (Shaker != null)
        {
            angelX = float.Parse(values[0]);
            angelY = float.Parse(values[1]);
            angelZ = float.Parse(values[2]);
            
            rotationVector = new Vector3(angelX,0, angelZ);

            x.text = angelX.ToString();
            y.text = angelY.ToString();
            z.text = angelZ.ToString();
            arrayLength.text = splitValues.Length.ToString();

            posX = float.Parse(values[3]);
            posY = float.Parse(values[4]);
            posZ = float.Parse(values[5]);
            

            xAcc.text = posX.ToString();
            yAcc.text = posY.ToString();
            zAcc.text = posZ.ToString();

            posX = posX - 9;

            shakerPos = new Vector3(posX, Smoother(posY), Smoother(posZ));
            
            currentAcc = shakerPos;

            val = values[6];
        }
    }

    private void LateUpdate()
    {
        previousAcc = new Vector3(accX,accY,accZ);
    }

    //When the game closes the port to the arduino will close as well
    //and turn off the LEDs
    private void OnDestroy()
    {
        //WriteToArduio("r");
        serial.Close();
    }

    
    // code for the remap function came from https://forum.unity.com/threads/re-map-a-number-from-one-range-to-another.119437/
    //The function below will re-map one range(Poteneometer values) to another range (Paddel position)
    float Remap(float value, float from1, float to1, float from2, float to2)
    {
        return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
    }

    //When the controller tilts the object will move in that direction
    public void ShakerMovement()
    {
        if (angelZ > 1 && angelZ < 30)
        {
            ShakerMove.transform.Translate(Vector3.left * Time.deltaTime * 2);
        }
        else if (angelZ < -1 && angelZ > -30)
        {
            ShakerMove.transform.Translate(Vector3.right * Time.deltaTime * 2);
        }

        if (angelX > 1 && angelX < 30)
        {
            ShakerMove.transform.Translate(Vector3.forward * Time.deltaTime * 2);
        }
        else if (angelX < -1 && angelX > -30)
        {
            ShakerMove.transform.Translate(Vector3.back * Time.deltaTime * 2);
        }
    }

    //The angle of the object will be equal to the angle of the controller
    public void ShakerRotaion()
    {
        Shaker.transform.eulerAngles = rotationVector;
    }

    public float Smoother(float axis)
    {
        float average;
        for (int i = 0; i < 4; i++)
        {
            movementList.Add(axis);
            num += movementList[i];
        }
        average = num / 4;
        for (int i = 0; i < 4; i++)
        {
            movementList.Remove(axis);
        }

        return average;
    }
}
