using UnityEngine.UI;
//This tells unity that I am using a UI canvas, which the buttons are on
using UnityEngine;
using UnityEngine.SceneManagement;
//This tells unity that I am using the scenemanagement to move between scenes 

public class Death : MonoBehaviour
{
    public Text Lives;

    public int LivesCount = 10;

    private void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Hazard")
        {
            LivesCount--;
            Lives.text = "Lives: " + LivesCount;

            if (LivesCount == 0)
            {
                SceneManager.LoadScene("Death");
            }
            //This is saying if the players health reaches 0 then go to the death scene
        }
        
    } 
	
}
