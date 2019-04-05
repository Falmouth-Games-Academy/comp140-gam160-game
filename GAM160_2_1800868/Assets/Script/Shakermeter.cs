using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Shakermeter : MonoBehaviour {

    [SerializeField]
    private Arduino arduino;
    [SerializeField]
    private Image shakeBar;

    public bool isShaken;

    private float originalYAxis;
    private float barScale;
    private float y;

    // Use this for initialization
    void Start () {
        originalYAxis = transform.position.y;
        barScale = 0;
	}
	
	// Update is called once per frame
	void Update () {

        if (Input.GetMouseButton(0))
        {
            float x = transform.position.x;
            float z = transform.position.z;

            y = Mathf.Lerp(-2, 2,Time.deltaTime);

            transform.position = new Vector3(0, y,0);
        }

        if (arduino.shakerPos.y < 0 || arduino.shakerPos.y > 0)
        {
            Debug.Log(arduino.shakerPos.y);
            shakeBar.rectTransform.localEulerAngles += new Vector3 (barScale * (Time.deltaTime * 0.1f),1,1);
        }

        if (barScale >= 1)
        {
            barScale = 1.0f;
            isShaken = true;
        }
	}
}
