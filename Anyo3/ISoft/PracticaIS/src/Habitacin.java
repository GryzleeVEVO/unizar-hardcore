import com.modeliosoft.modelio.javadesigner.annotations.objid;

@objid ("7afcafa2-0872-4397-888c-aef6c9985e19")
public class Habitación {
    @objid ("3a43c437-e0d5-43a9-8575-c8230da62a65")
    public String identificador;

    @objid ("2abbc59c-399f-4346-a2be-532dc0f29091")
    public String descripcion;

    @objid ("1dd13dce-20e0-4a06-9ca1-a631de24317e")
    public int maxOcupantes;

    @objid ("f479f11c-319a-4465-86f5-0a35b5b7fae2")
    public float precio;

    @objid ("466f660d-26f7-4f6e-a0d9-f450aed13dce")
    public float recargo;

    @objid ("e92e60a4-a603-452f-b323-09d4b6816681")
    public Habitación(final String identificador, final String descripcion, final int maxOcupantes, final float precio, final float recargo) {
    }

    @objid ("8424cfc7-9c43-4cb1-819b-597680ca6333")
    public void Operation() {
    }

}
