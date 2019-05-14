using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera_Scene : MonoBehaviour
{
    bool CameraTrig,Zoom ;

    private Animator animator;
    // Use this for initialization
    void Start()
    {
        animator = GetComponent<Animator>();
        CameraTrig = true ;
        Zoom = false;
    }

    // Update is called once per frame
    void Update ()
    {
        if (Input.GetKeyDown(KeyCode.Space)&&(CameraTrig == true))
        {
        
            animator.SetBool("Look_Book", CameraTrig);
            CameraTrig = false;
        }
        if (Input.GetKeyDown(KeyCode.Space) && (CameraTrig == false))
        {
         
            animator.SetBool("Look_Book", CameraTrig);
            CameraTrig = true ;
        }
        if (Input.GetKeyDown(KeyCode.Space) && (Zoom == false))
        {

            animator.SetBool("Zoom", Zoom);
            CameraTrig = true;
        }
    }
}
