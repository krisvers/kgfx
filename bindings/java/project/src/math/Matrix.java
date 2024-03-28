package math;

import java.util.Arrays;

public class Matrix {
	public float[][] values;

	public Matrix(int w, int h) {
		this.values = new float[w][h];
		identity();
	}

	public static int bytesize(int w, int h) {
		return w * h * Float.BYTES;
	}

	public int size() {
		return values.length * values[0].length;
	}

	public int bytesize() {
		return size() * Float.BYTES;
	}

	public void zero() {
		check();

		for (float[] array : this.values) {
			Arrays.fill(array, 0);
		}
	}

	public void identity() {
		zero();
		values[0][0] = 1;
		values[1][1] = 1;
		values[2][2] = 1;
		values[3][3] = 1;
	}

	public Matrix add(Matrix matrix) {
		check();
		check(matrix);

		if (matrix.values.length != this.values.length || matrix.values[0].length != this.values[0].length) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);

		for (int i = 0; i < this.values.length; ++i) {
			for (int j = 0; j < this.values[0].length; ++j) {
				n.values[i][j] = this.values[i][j] + matrix.values[i][j];
			}
		}

		return n;
	}

	public Matrix sub(Matrix matrix) {
		check();
		check(matrix);

		if (matrix.values.length != this.values.length || matrix.values[0].length != this.values[0].length) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);

		for (int i = 0; i < this.values.length; ++i) {
			for (int j = 0; j < this.values[0].length; ++j) {
				n.values[i][j] = this.values[i][j] - matrix.values[i][j];
			}
		}

		return n;
	}

	public Matrix scale(float scalar) {
		if (scalar == 0) {
			zero();
			return this;
		}

		check();

		Matrix n = new Matrix(this.values.length, this.values[0].length);

		for (int i = 0; i < this.values.length; ++i) {
			for (int j = 0; j < this.values[0].length; ++j) {
				n.values[i][j] = this.values[i][j] * scalar;
			}
		}

		return n;
	}

	public Matrix scaleAniso(float x, float y, float z) {
		if (x == 0 && y == 0 && z == 0) {
			zero();
			return this;
		}

		check();

		Matrix n = new Matrix(this.values.length, this.values[0].length);

		for (int i = 0; i < this.values.length; ++i) {
			for (int j = 0; j < this.values[0].length; ++j) {
				float s = 1;
				if (i == 0) {
					s = x;
				} else if (i == 1) {
					s = y;
				} else if (i == 2) {
					s = z;
				}

				n.values[i][j] = this.values[i][j] * s;
			}
		}

		return n;
	}

	public Matrix scaleAniso(Vector vector) {
		if (vector.values.length != 3) {
			throw new RuntimeException("Vector length must be 3");
		}

		if (vector.values[0] == 0 && vector.values[1] == 0 && vector.values[2] == 0) {
			zero();
			return this;
		}

		check();

		Matrix n = new Matrix(this.values.length, this.values[0].length);

		for (int i = 0; i < this.values.length; ++i) {
			for (int j = 0; j < this.values[0].length; ++j) {
				if (i < vector.values.length) {
					n.values[i][j] = this.values[i][j] * vector.values[i];
				} else {
					n.values[i][j] = this.values[i][j];
				}
			}
		}

		return n;
	}

	public Matrix mul(Matrix matrix) {
		check();
		check(matrix);

		if (matrix.values.length != this.values.length || matrix.values[0].length != this.values[0].length) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		for (int c = 0; c < this.values.length; ++c) {
			for (int r = 0; r < this.values[0].length; ++r) {
				n.values[c][r] = 0;
				for (int k = 0; k < this.values[0].length; ++k) {
					n.values[c][r] += this.values[k][r] * matrix.values[c][k];
				}
			}
		}

		return n;
	}

	public Vector mulVector(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length || vector.values.length != this.values[0].length) {
			throw new RuntimeException("Matrix dimensions do not match vector length");
		}

		Vector n = new Vector(this.values[0].length);

		for (int i = 0; i < this.values.length; ++i) {
			for (int k = 0; k < this.values[0].length; ++k) {
					n.values[i] += this.values[k][i] * vector.values[k];
			}
		}

		return n;
	}

	public Matrix translate(float x, float y, float z) {
		check();

		if (this.values.length != 4 || this.values[0].length != 4) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		n.identity();

		n.values[3][0] = x;
		n.values[3][1] = y;
		n.values[3][2] = z;

		return n;
	}

	public Matrix translate(Vector vector) {
		check();

		if (this.values.length != 4 || this.values[0].length != 4) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		n.identity();

		n.values[3][0] = vector.values[0];
		n.values[3][1] = vector.values[1];
		n.values[3][2] = vector.values[2];

		return n;
	}

	public Matrix translateInPlace(float x, float y, float z) {
		check();

		if (this.values.length != 4 || this.values[0].length != 4) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		for (int i = 0; i < this.values.length; ++i) {
			n.values[3][i] += row(i).mulInner(new Vector(x, y, z, 0));
		}

		return n;
	}

	public Matrix translateInPlace(Vector vector) {
		check();

		if (this.values.length != 4 || this.values[0].length != 4) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		float x = vector.values[0];
		float y = vector.values[1];
		float z = vector.values[2];

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		for (int i = 0; i < this.values.length; ++i) {
			n.values[3][i] += row(i).mulInner(new Vector(x, y, z, 0));
		}

		return n;
	}

	public Matrix rotateX(float angle) {
		check();

		if (this.values.length < 3 || this.values[0].length < 3) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		n.identity();

		float s = (float) Math.sin(angle);
		float c = (float) Math.cos(angle);

		n.values[1][1] = c;
		n.values[1][2] = s;
		n.values[2][1] = -s;
		n.values[2][2] = c;

		return this.mul(n);
	}

	public Matrix rotateY(float angle) {
		check();

		if (this.values.length < 3 || this.values[0].length < 3) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		n.identity();

		float c = (float) Math.cos(angle);
		float s = (float) Math.sin(angle);

		n.values[0][0] = c;
		n.values[0][2] = -s;
		n.values[2][0] = s;
		n.values[2][2] = c;

		return this.mul(n);
	}

	public Matrix rotateZ(float angle) {
		check();

		if (this.values.length < 3 || this.values[0].length < 3) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		n.identity();

		float c = (float) Math.cos(angle);
		float s = (float) Math.sin(angle);

		n.values[0][0] = c;
		n.values[0][1] = s;
		n.values[1][0] = -s;
		n.values[1][1] = c;

		return this.mul(n);
	}

	public Matrix rotate(Vector vector) {
		check();

		if (this.values.length < 3 || this.values[0].length < 3 || vector.values.length < 3) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		return rotate(vector.values[0], vector.values[1], vector.values[2]);
	}

	public Matrix rotate(float x, float y, float z) {
		check();

		if (this.values.length < 3 || this.values[0].length < 3) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		Matrix n = new Matrix(this.values.length, this.values[0].length);
		n.identity();

		return n.rotateX(x).rotateY(y).rotateZ(z);
	}

	public void set(float[][] matrix) {
		check();
		check(matrix);

		if (matrix.length != this.values.length || matrix[0].length != this.values[0].length) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		for (int i = 0; i < this.values.length; ++i) {
			System.arraycopy(matrix[i], 0, this.values[i], 0, this.values[i].length);
		}
	}

	public void set(Matrix matrix) {
		check();
		check(matrix);

		if (matrix.values.length != this.values.length || matrix.values[0].length != this.values[0].length) {
			throw new RuntimeException("Matrix dimensions do not match");
		}

		for (int i = 0; i < this.values.length; ++i) {
			System.arraycopy(matrix.values[i], 0, this.values[i], 0, this.values[i].length);
		}
	}

	public Matrix copy() {
		check();
		Matrix matrix = new Matrix(this.values.length, this.values[0].length);
		matrix.set(this);
		return matrix;
	}

	public Vector[] toVectors() {
		check();

		Vector[] vectors = new Vector[this.values.length];
		for (int i = 0; i < this.values.length; ++i) {
			vectors[i] = new Vector(this.values[i].length);
			System.arraycopy(this.values[i], 0, vectors[i].values, 0, this.values[i].length);
		}

		return vectors;
	}

	public Vector row(int row) {
		check();
		if (row < 0 || row >= this.values.length) {
			throw new RuntimeException("Row index out of bounds");
		}

		Vector vector = new Vector(this.values[row].length);
		System.arraycopy(this.values[row], 0, vector.values, 0, this.values[row].length);
		return vector;
	}

	public Vector column(int col) {
		check();
		if (col < 0 || col >= this.values.length) {
			throw new RuntimeException("Column index out of bounds");
		}

		Vector vector = new Vector(this.values.length);
		for (int i = 0; i < this.values.length; ++i) {
			vector.values[i] = this.values[i][col];
		}

		return vector;
	}

	public static Matrix perspective(float fov, float aspect, float near, float far) {
		Matrix matrix = new Matrix(4, 4);
		matrix.zero();
		final float a = (float) (1.f / Math.tan(Math.toRadians(fov / 2.f)));

		matrix.values[0][0] = a / aspect;
		matrix.values[1][1] = a;
		matrix.values[2][2] = -((far + near) / (far - near));
		matrix.values[2][3] = -1.f;
		matrix.values[3][2] = -((2.f * far * near) / (far - near));

		return matrix;
	}

	public static Matrix ortho(float left, float right, float top, float bottom, float near, float far) {
		Matrix matrix = new Matrix(4, 4);
		matrix.zero();

		matrix.values[0][0] = 2 / (right - left);
		matrix.values[1][1] = 2 / (top - bottom);
		matrix.values[2][2] = -2 / (far - near);

		matrix.values[3][0] = -(right + left) / (right - left);
		matrix.values[3][1] = -(top + bottom) / (top - bottom);
		matrix.values[3][2] = -(far + near) / (far - near);
		matrix.values[3][3] = 1;

		return matrix;
	}

	public float[] toFloatArray() {
		check();

		float[] array = new float[this.values.length * this.values[0].length];
		for (int i = 0; i < this.values.length; ++i) {
			System.arraycopy(this.values[i], 0, array, i * this.values[i].length, this.values[i].length);
		}

		return array;
	}

	public float[][] toFloat2DArray() {
		check();

		float[][] array = new float[this.values.length][this.values[0].length];
		for (int i = 0; i < this.values.length; ++i) {
			System.arraycopy(this.values[i], 0, array[i], 0, this.values[i].length);
		}

		return array;
	}

	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Matrix matrix)) {
			return false;
		}
		if (this == obj) {
			return true;
		}

		if (matrix.values.length != this.values.length) {
			return false;
		}

		for (int i = 0; i < this.values.length; ++i) {
			for (int j = 0; j < this.values[0].length; ++j) {
				if (this.values[i][j] != matrix.values[i][j]) {
					return false;
				}
			}
		}

		return true;
	}

	@Override
	public String toString() {
		check();
		StringBuilder str = new StringBuilder();
		for (int i = 0; i < this.values.length; ++i) {
			str.append("[ ");
			for (int j = 0; j < this.values[0].length; ++j) {
				str.append(this.values[i][j]);
				if (j < this.values[0].length - 1) {
					str.append(", ");
				}
			}
			str.append(" ]\n");
		}
		return str.toString();
	}

	private void check() {
		if (this.values == null) {
			throw new RuntimeException("Matrix not initialized");
		}
	}

	private void check(Matrix matrix) {
		if (matrix == null) {
			throw new RuntimeException("Matrix not initialized");
		}
	}

	private void check(float[][] matrix) {
		if (matrix == null) {
			throw new RuntimeException("Matrix not initialized");
		}
	}

	private void check(Vector vector) {
		if (vector == null) {
			throw new RuntimeException("Matrix not initialized");
		}
	}
}
