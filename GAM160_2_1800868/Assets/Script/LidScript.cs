using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LidScript : MonoBehaviour {

    [SerializeField]
    private Arduino arduino;

    public bool lidIsOpen;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (arduino != null)
        {
            //when the button is pressed the lid will close else it'll be open
            if (arduino.val == "e")
            {
                lidIsOpen = false;
                transform.position = new Vector3(0.012f, 1.816f, -0.04f);
                transform.eulerAngles = new Vector3(-90, -90, 89.99999f);
            }
            else
            {
                lidIsOpen = true;
                transform.position = new Vector3(-0.97f, 2.1f, -0.04f);
                transform.eulerAngles = new Vector3(6.724f, -90.00001f, 90.00001f);
            }
        }
    }
}
