using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LidScript : MonoBehaviour {

    [SerializeField]
    private Arduino arduino;

    private Vector3 currentPos;
    private Vector3 defaltRotation;

    public bool lidIsOpen;

    // Use this for initialization
    void Start () {
        defaltRotation = transform.eulerAngles;
	}
	
	// Update is called once per frame
	void Update () {

        

        if (arduino != null)
        {
            //when the button is pressed the lid will appear else it will disapear
            if (arduino.val == "e")
            {
                lidIsOpen = false;
                GetComponent<MeshRenderer>().enabled = true;
            }
            else
            {
                lidIsOpen = true;
                GetComponent<MeshRenderer>().enabled = false;
            }
        }

        
    }
}
