using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
//This tells the project, that we are using the build settings to move between scenes

public class Pause : MonoBehaviour {

	public static bool GameIsPaused = false;
    //This is saying to start with the game is not paused, so it is false
    
    public GameObject PauseMenu;
    //This opens the information in the inspcetor, which allows me to drag the pause canvas into the box of the canvas that is always visible


    void Update()
    {
        if (Input.GetKeyUp(KeyCode.P))
        {
            if (GameIsPaused)
            {
                Resume();
            }
            else
            {
                PauseGame();
            }
        }
    }
    //This code is saying, if the player presses P and the game is paused. Then resume the game. If the game is not paused and the P was pressed, then pause the game

    void Start ()
    {
        PauseMenu.SetActive(false);
	}

    public void Resume()
    {
        PauseMenu.SetActive(false);
        Time.timeScale = 1f;
        GameIsPaused = false;
    }
    //This code is saying, when resuming the game. The pause menu has to be set to false, which means that the pause menu has got to disappear. The game has to start playing again, which means 
    //that the game has to unfreeze. This also allows the player to click the resume button.

    public void PauseGame()
    {
        PauseMenu.SetActive(true);
        Time.timeScale = 0f;
        GameIsPaused = true;
    }
    //This code is saying, when pausing the game. The pause menu has to be set to true, which means that the pause menu has got to appear. The game has to stop playing, which means 
    //that the game has to freeze. This only happens when the player clicks the P button.


    public void MainMenu()
    {
        Time.timeScale = 1f;
        SceneManager.LoadScene("MainMenu");
    }
    //This code is saying, if the player presses main menu button then unfreeze the game, and take the player to the main menu page

}
