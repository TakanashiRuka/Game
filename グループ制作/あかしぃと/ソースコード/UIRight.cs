using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIRight : MonoBehaviour {

    public Charcter Charcter;

    Vector3 Move;
    Vector3 PosInit;

	// Use this for initialization
	void Start ()
    {
        Move = new Vector3(20.0f, 0.0f, 0.0f);

        PosInit = this.transform.position;
    }
	
	// Update is called once per frame
	void Update ()
    {
        bool WalkCheck;

        WalkCheck = Charcter.GetWalk();

        if(WalkCheck)
        {
            if (this.transform.position.x <= PosInit.x + 350)
            {
                this.transform.position += Move;
            }
      
        }
        else
        {
            if(this.transform.position != PosInit)
            {
                this.transform.position -= Move;
            }
        }
		
	}
}
