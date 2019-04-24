using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LidScript : MonoBehaviour {

    [SerializeField]
    private Arduino arduino;

    private Vector3 currentPos;

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
                
            }
            else
            {
                lidIsOpen = true;
                
            }
        }

        
    }
}
