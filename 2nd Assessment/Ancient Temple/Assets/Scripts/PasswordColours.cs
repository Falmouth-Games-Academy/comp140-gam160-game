using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; 


public class PasswordColours: MonoBehaviour {

    public Canvas passwordCanvas;
    public GameObject[] passwordOptions = new GameObject[0];
    //This tells unity to start with an empty array 

    void OnTriggerEnter(Collider other)
    {
        if (passwordCanvas == null)
            return;

        if (other.tag != "Player")
            return;

        passwordCanvas.gameObject.SetActive(true);
        //This allows the canvas with the text to pop up
        Time.timeScale = 0f;
        //This freezes the game

        // disable all the gameobjects in the list of password options
        foreach(GameObject itemImage in passwordOptions)
        {
            itemImage.SetActive(false);
        }

        // enable one of them
        int theItemToEnable = Random.Range(0, passwordOptions.Length); // a random index within passwordOptions
        passwordOptions[theItemToEnable].SetActive(true);
    }
}
