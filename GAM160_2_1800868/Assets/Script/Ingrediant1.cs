using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ingrediant1 : MonoBehaviour {

    [SerializeField]
    private Arduino arduino;
    [SerializeField]
    private GameObject liquid;
    [SerializeField]
    private Transform spawnPoint;
    [SerializeField]
    private string ingrediantID;
    [SerializeField]
    private float rotateSpeed;

    private Quaternion idelAng;
    private Quaternion setAngle;
    private float constXaxis;

    // Use this for initialization
    void Start () {
        constXaxis = transform.eulerAngles.x;
	}
	
	// Update is called once per frame
	void Update () {

        if (arduino != null)
        {

            if (arduino.values.Contains(ingrediantID))
            {
                pour();
            }
            else
            {
                transform.eulerAngles = new Vector3(constXaxis, transform.eulerAngles.y, transform.eulerAngles.z);
            }

        }

        /*if (Input.GetMouseButton(0))
        {
            pour();
        }
        else
            transform.eulerAngles = new Vector3(constXaxis, transform.eulerAngles.y, transform.eulerAngles.z);
            */


    }

    void pour()
    {
        transform.eulerAngles = new Vector3(-90, transform.eulerAngles.y, transform.eulerAngles.z);
        GameObject drop = Instantiate(liquid, spawnPoint.position, Quaternion.identity);
        Destroy(drop, 2);
    }
}
