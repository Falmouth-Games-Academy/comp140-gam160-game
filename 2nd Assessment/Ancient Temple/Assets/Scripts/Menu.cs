using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
//This tells unity that I am using a UI canvas, which the buttons are on
using UnityEngine.SceneManagement;
//This tells unity that I am using the scenemanagement to move between scenes 
//All these scenes have to be in the build settings, to move between them

public class Menu : MonoBehaviour {

    public void StartNewGame()
    {
        SceneManager.LoadScene("Level 1");
        //This loads the first level when the start button is clicked 
    }

    public void HowToPlay()
    {
        SceneManager.LoadScene("Instructions");
        //This loads the instructions page when the start button is clicked 
    }

    public void TheMenu()
    {
        SceneManager.LoadScene("MainMenu");
        //This loads the main menu scene when the start button is clicked 
    }

    public void Quit()
    {
        Application.Quit();
        //This closes the software when the start button is clicked 
    }
}
