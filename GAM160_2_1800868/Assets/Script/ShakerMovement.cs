using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.Events;

public class ShakerMovement : MonoBehaviour {

    [SerializeField]
    private bool forTesting;

    private bool isOpen;
    private Transform t;

    public UnityEvent shaker;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

        //If forTesting is true then the player will control the shaker via the keyboard else it will invoke the registerted function
        if (forTesting)
            transform.Translate(Input.GetAxis("Horizontal"), 0, Input.GetAxis("Vertical"));
        else
            shaker.Invoke();

        /*
        if (Input.GetMouseButtonDown(0) && GetComponent<BoxCollider>() != null)
        {
            if (!isOpen)
            {
                isOpen = true;
                GetComponent<BoxCollider>().enabled = true;
            }
            else
            {
                isOpen = false;
                GetComponent<BoxCollider>().enabled = false;
            }

            Debug.Log(isOpen);
        }
        */
	}
}
