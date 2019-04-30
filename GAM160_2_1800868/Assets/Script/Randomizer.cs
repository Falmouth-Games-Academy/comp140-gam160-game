using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class Randomizer : MonoBehaviour {
    
    [SerializeField]
    private Image Layer;
    [SerializeField]
    private Transform layerBackground;
    [SerializeField]
    public List<Image> Layers = new List<Image>();
    public List<int> layerNums = new List<int>();


    string[] ingrediantsList = new string[] { "Sugar Syrup", "Lemon Juice", "Lime Juice", "Dry Gin", "Vodka"};
    int randomNum;

    // Use this for initialization
    void Start () {
        RandomLayers();
	}

    public void RandomLayers()
    {
        //For every layer it will be given a value which is diasplayed on the UI list and added a list for the check script
        foreach (Image layer in Layers)
        {
            string String = ingrediantsList[0].ToString();
            randomNum = Random.Range(0, ingrediantsList.Length);

            layer.GetComponentInChildren<Text>().text = ingrediantsList[randomNum];
            layerNums.Add(randomNum);
        }
    }

    //When called the previous list will be removed
    public void RemoveOldList()
    {
        layerNums.Clear();
    }
}
