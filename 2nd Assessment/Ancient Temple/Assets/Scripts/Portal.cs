using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Portal : MonoBehaviour {

    public string levelToLoad;

    private void OnTriggerEnter(Collider other)
    {
        if (string.IsNullOrEmpty(levelToLoad))
            return;

        if (other.tag != "Player")
            return;

        SceneManager.LoadScene(levelToLoad);
        //This code is saying if the player touches the object that go to that scene
    }
}
