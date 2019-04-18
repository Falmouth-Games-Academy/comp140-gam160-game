using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class InstructionsControl : MonoBehaviour {

    public Canvas dangersCanvas;
    public Canvas controlsCanvas;
    public Canvas informationCanvas;

	public void instructionsInfo()
    {
        informationCanvas.gameObject.SetActive(true);
        controlsCanvas.gameObject.SetActive(false);
        dangersCanvas.gameObject.SetActive(false);
    }

    public void instructionsDangers()
    {
        informationCanvas.gameObject.SetActive(false);
        controlsCanvas.gameObject.SetActive(false);
        dangersCanvas.gameObject.SetActive(true);
    }

    public void instructionsControls()
    {
        informationCanvas.gameObject.SetActive(false);
        controlsCanvas.gameObject.SetActive(true);
        dangersCanvas.gameObject.SetActive(false);
    }
}
