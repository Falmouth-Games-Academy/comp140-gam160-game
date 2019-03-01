using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameOver : MonoBehaviour {

    public GameObject GameOverMenu;
    //This code opens the information into the inspector, which allows me to drag the gameover canvas onto the canvas that is visibal at all times

    public static bool GameIsOver = false;
    //This code is saying when the game is playing, the gameover menu is not active

    public int endGame = 10;
    //This is the score that the players has to get to end the game and win

    public Text Player1;
    public Text Player2;
    //This opens the information into the inspector, which allows me to drag the scores for each player into the correct box

    void Update ()
    {
        if (Player1.text == endGame.ToString())
        {
            GameOverEnd();
        }

        if (Player2.text == endGame.ToString())
        {
            GameOverEnd();
        }
    }
    //This code is saying, when ever a player reaches the certain score call this function

    void GameOverEnd()
    {
        GameOverMenu.SetActive(true);
        Time.timeScale = 0f;
    }
    //This function is saying, pop up the gameover menu which as tops the games

    public void MainMenu()
    {
        Time.timeScale = 1f;
        SceneManager.LoadScene("MainMenu");
    }

    public void Quit()
    {
        Application.Quit();
    }
    //These two codes above allow the player to click on the buttons, which are on the gameover menu that take the user to other scenes or quits the game
}
