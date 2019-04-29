using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ChangeScene : MonoBehaviour {

    [SerializeField]
    private string scene;

    //The method below will chnge the scene depending on what's been set
    public void SwitchtoScene()
    {
        SceneManager.LoadScene(scene);
    }
}
