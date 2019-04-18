using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; 


public class PasswordColours: MonoBehaviour {

    public Canvas passwordCanvas;


    void OnTriggerEnter(Collider other)
    {
        if (passwordCanvas == null)
            return;

        if (other.tag != "Player")
            return;

        passwordCanvas.gameObject.SetActive(true);
        Time.timeScale = 0f;

    }
}
