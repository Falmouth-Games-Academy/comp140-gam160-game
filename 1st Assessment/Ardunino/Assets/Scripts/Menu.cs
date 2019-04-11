using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
//This tells the project, that will are using the build settings to move between scenes

public class Menu : MonoBehaviour {

	public void StartMenu()
    {
        SceneManager.LoadScene("MainMenu");
    }
    //This code allows the main menu scene to open, when the button is clicked

    public void StartGame()
    {
        SceneManager.LoadScene("Pong");
        Time.timeScale = 1f;
    }
    //This code allows the pong game scene to open, when the button is clicked. This will also unfreeze the game. 

    public void Instructions()
    {
        SceneManager.LoadScene("HowToPlay");
    }
    //This code allows the instructions scene to open, when the button is clicked. 

    public void Quit()
    {
        Application.Quit();
    }
    //This code allows build of the game to be closed, when the button is clicked. 

}
