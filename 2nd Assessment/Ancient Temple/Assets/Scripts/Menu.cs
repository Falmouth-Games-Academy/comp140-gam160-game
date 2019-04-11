using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Menu : MonoBehaviour {

    public void StartNewGame()
    {
        SceneManager.LoadScene("Level 1");
    }

    public void HowToPlay()
    {
        SceneManager.LoadScene("Instructions");
    }

    public void TheMenu()
    {
        SceneManager.LoadScene("MainMenu");
    }

    public void Quit()
    {
        Application.Quit();
    }
}
