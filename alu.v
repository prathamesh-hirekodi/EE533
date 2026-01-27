`timescale 1ns /1ps

module alu(
    input [31:0] A,
    input [31:0] B,
    input [2:0] ALUop,
    output reg [31:0] O,
    output Zero
);

assign Zero = (O == 32'b0);


always @(*) begin
    case (ALUop)
        3'b000: O = A + B;
        3'b001: O = A - B;
        3'b010: O = A | B;
        3'b011: O = A & B;
        3'b101: O = A << B[4:0];
        default: O = 32'b0;
    endcase
end
endmodule